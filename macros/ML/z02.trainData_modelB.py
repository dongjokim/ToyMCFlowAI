import sys, os
import numpy as np
import matplotlib.pyplot as plt
# define the function
import tensorflow as tf
#import shap
from tensorflow.keras.utils import plot_model
from sklearn import preprocessing
import keras
from keras.models import Sequential
from keras.layers import Input, Dense, Flatten, Dropout, Activation, Conv1D, Conv1DTranspose, Conv2D, Conv2DTranspose, MaxPooling2D, UpSampling2D
from tensorflow.keras import models, layers, utils, backend as K
from plot_mymodel import *
from model_B import *
from glob import glob
from tqdm import tqdm

res = 32;

outdir = 'images_out/'
for fn in tqdm(glob(outdir+"images*.npz")):
	io = np.load(fn);
	data = io['arr_0']
	obs = io['arr_1']

	#print('We have {} events and {} true v2 '.format(data.shape[0],obs.shape[0]))
	# need to do X[ievt]=3x32x32 per event (total 1000evt)
	# the data coming out of previous commands is a list of 2D arrays. We want a 3D np array (n_events, xpixels, ypixels)
	ndim = res*res; #1D convs
	x = data.reshape(data.shape[0],ndim,3) #this shape allows easy normalization of the image channel
	#ndim = res; #2D convs
	#x = data.reshape(data.shape[0],ndim,ndim,3)
	try:
		X = np.concatenate((X,x));
		y = np.concatenate((y,obs));
	except NameError:
		X = x;
		y = obs;
		
	tqdm.write("Loaded {}, {}".format(fn,X.shape));

#print("----",X[:,:,0].shape);
#print("----",X[:,:,1].shape);
#preprocessing.normalize(X,norm='l2',copy=False); #L2 normalization scheme
preprocessing.normalize(X[:,:,0],norm='l2',copy=False);
preprocessing.normalize(X[:,:,1],norm='l2',copy=False);
preprocessing.normalize(X[:,:,2],norm='l2',copy=False);
#y = preprocessing.normalize(y,norm='l2');
y = y[:,0] # v2 only

n_features = ndim # per event[:,0]
print(n_features);

#1D conv
#model_cnn = Sequential([
#	Input(shape=(ndim,3)),
#	Conv1D(filters=64,kernel_size=3,padding="same",activation="relu"),
#	Dropout(rate=0.1),
#	Conv1D(filters=64,kernel_size=3,padding="same",activation="relu"),
#
#	Conv1DTranspose(filters=32,kernel_size=3,padding="same",activation="relu"),
#	Dropout(rate=0.1),
#	Conv1DTranspose(filters=64,kernel_size=3,padding="same",activation="relu")
#]);

#2D conv
X = X.reshape(X.shape[0],res,res,3);
#print("**",X.shape);

X_case = X[(y < 0.05),:,:,:];
print("****",X.shape);

split = int(0.7*X_case.shape[0]);
X_train = X_case[:split];
X_test = X_case[split:];

#model_cnn = Sequential([
#	#Input(shape=(res,res,3)),
#	Conv2D(filters=2,kernel_size=3,padding="same",activation="relu",input_shape=X.shape[1:]),
#	Dropout(rate=0.1),
#	Conv2D(filters=4,kernel_size=3,padding="same",activation="relu"),
#
#	Conv2DTranspose(filters=3,kernel_size=4,padding="same",activation="relu"),
#	Dropout(rate=0.1),
#	Conv2DTranspose(filters=3,kernel_size=2,padding="same",activation="relu")
#]);

model_cnn = Sequential([
	Conv2D(filters=16,kernel_size=(3,3),padding="same",activation="relu",input_shape=X_train.shape[1:]),
	Dropout(rate=0.1),
	#MaxPooling2D((2,2),padding="same"),
	Conv2D(filters=8,kernel_size=(3,3),padding="same",activation="relu"),
	#MaxPooling2D((2,2),padding="same"),
	Conv2D(filters=8,kernel_size=(3,3),padding="same",activation="relu"),
	Dense(48,activation="relu"),
	Dense(32,activation="relu"),
	Dense(48,activation="relu"),
	Conv2DTranspose(filters=8,kernel_size=(3,3),padding="same",activation="relu"),
	#UpSampling2D((2,2)),
	Conv2DTranspose(filters=8,kernel_size=(3,3),padding="same",activation="relu"),
	#UpSampling2D((2,2)),
	#Dropout(rate=0.1),
	Conv2DTranspose(filters=16,kernel_size=(3,3),padding="same",activation="relu"),
	Conv2DTranspose(filters=3,kernel_size=(3,3),padding="same"),
]);

#model_cnn = Sequential([
#	Dense(128,activation="relu",input_shape=X_train.shape[1:]),
#	#Dropout(rate=0.1),
#	#MaxPooling2D((2,2),padding="same"),
#	Dense(64,activation="relu"),
#	#MaxPooling2D((2,2),padding="same"),
#	Dense(32,activation="relu"),
#
#	Dense(32,activation="relu"),
#	#UpSampling2D((2,2)),
#	Dense(64,activation="relu"),
#	#UpSampling2D((2,2)),
#	#Dropout(rate=0.1),
#	Dense(128,activation="relu"),
#	Dense(3,activation="relu"),
#]);

plot_model(model_cnn, to_file='figs/modelB_plot.png', show_shapes=True, show_layer_names=True)
#visualize_nn(model_cnn)

# Compile model
model_cnn.compile(optimizer='adam',loss=tf.keras.losses.MeanSquaredError());
model_cnn.summary();

#exit();
#epochs=10 to not overfit
#batch_size=32 (PRD)
#history_cnn = model_cnn.fit(X, y, validation_split=0.3, epochs=20, batch_size=32, shuffle=True, verbose=1, validation_steps=1)
history_cnn = model_cnn.fit(X_train, X_train, epochs=50, batch_size=32, shuffle=True, verbose=1, validation_steps=1,validation_data=(X_test,X_test))

#evaluate the model for the training set and retrieve the anomaly threshold value
train_pred = model_cnn.predict(X_train);
X_train = X_train.reshape(X_train.shape[0],res*res*3);
train_pred = train_pred.reshape(train_pred.shape[0],res*res*3);
train_mae = np.mean(np.abs(train_pred-X_train),axis=1);

test_pred = model_cnn.predict(X_test);
X_test = X_test.reshape(X_test.shape[0],res*res*3);
test_pred = test_pred.reshape(test_pred.shape[0],res*res*3);
test_mae = np.mean(np.abs(test_pred-X_test),axis=1);

X_other = X[(y > 0.15),:,:,:];
other_pred = model_cnn.predict(X_other);
X_other = X_other.reshape(X_other.shape[0],res*res*3);
other_pred = other_pred.reshape(other_pred.shape[0],res*res*3);
other_mae = np.mean(np.abs(other_pred-X_other),axis=1);

m1 = np.min([train_mae.min(),test_mae.min(),other_mae.min()]);
m2 = np.max([train_mae.max(),test_mae.max(),other_mae.max()]);
bins = np.linspace(m1,m2,50);
plt.hist(train_mae,bins,color="blue",alpha=0.5);
plt.hist(test_mae,bins,color="green",alpha=0.5);
plt.hist(other_mae,bins,color="red",alpha=0.5);
plt.show();

#try:
#	model_dir='trained_modelB/'
#	os.mkdir(model_dir);
#except FileExistsError:
#	pass;
#model_cnn.save(model_dir+'cnn.h5')
#np.savez(model_dir+'training_histories.npz', [ history.history for history in [ history_cnn ]])

