import sys, os
import numpy as np
import matplotlib.pyplot as plt
# define the function
import tensorflow as tf
#import shap
from tensorflow.keras.utils import plot_model
from sklearn import preprocessing,model_selection
import keras
from keras.models import Sequential
from keras.layers import Dense, Flatten, Dropout, Activation, Conv2D, MaxPooling2D
from tensorflow.keras import models, layers, utils, backend as K
from plot_mymodel import *
from model_B import *
from glob import glob

#tuning
import keras_tuner

outdir = 'images_out/'
for fn in glob(outdir+"images*.npz"):
	io = np.load(fn);
	data = io['arr_0']
	obs = io['arr_1']

	print('We have {} events and {} true v2 '.format(data.shape[0],obs.shape[0]))
	# need to do X[ievt]=3x32x32 per event (total 1000evt)
	# the data coming out of previous commands is a list of 2D arrays. We want a 3D np array (n_events, xpixels, ypixels)
	ndim = 3*32*32
	#ndim = 1*32*32
	x = data.reshape(data.shape[0],ndim)
	try:
		X = np.concatenate((X,x));
		y = np.concatenate((y,obs));
	except NameError:
		X = x;
		y = obs;
		
	print("Loaded {}".format(fn),X.shape);

preprocessing.normalize(X,norm='l2',copy=False); #L2 normalization scheme
#y = preprocessing.normalize(y,norm='l2');
y = y[:,0] # v2 only

#exec(open("model_B.py").read())

X_train,X_test,y_train,y_test = model_selection.train_test_split(X,y,test_size=0.2);

n_features = ndim # per event[:,0]

#BEST: tanh (first), 128, 512, 256

def build_model(hp):
	activation = hp.Choice('activation',['relu','tanh']);
	units1 = hp.Choice('units1',[128,256,512]);
	units2 = hp.Choice('units2',[128,256,512]);
	units3 = hp.Choice('units3',[128,256,512]);
	model_cnn = Sequential(name="DeepNN", layers=[
		### 
		Dense(name="h",input_dim=n_features,
		             units=128, #128
		             activation=activation),
		#### hidden layer 1
		Dense(name="h1",units=units1, #256
		             activation='relu'),
		### hidden layer 2
		Dense(name="h2",units=units2,
		             activation='relu'),
		### hidden layer 3
		Dense(name="h3",units=units3,
		             activation='relu'),
		Dense(name="output", units=1, activation='linear')
	])
	
	#model_cnn.summary()
	# Compile model
	model_cnn.compile(optimizer='adam',
		#loss=tf.keras.losses.MeanSquaredError(hp.Choice('lr',[1e-2,1e-3,1e-4])));
		loss=tf.keras.losses.MeanSquaredError());

	return model_cnn

#model_cnn = build_model(hp)
print("Searching best model configuration.");

tuner = keras_tuner.RandomSearch(
	build_model,
	objective='val_loss',
	project_name='vnpred',
	max_trials=10);

tuner.search(X_train,y_train,epochs=20,validation_data=(X_test,y_test));
best_model = tuner.get_best_models()[0];

print("Training the best model.");

#plot_model(model_cnn, to_file='figs/modelB_plot.png', show_shapes=True, show_layer_names=True)
#visualize_nn(model_cnn)

#epochs=10 to not overfit
#batch_size=32 (PRD)
#history_cnn = model_cnn.fit(X, y, validation_split=0.2, epochs=20, batch_size=32, shuffle=True, verbose=1, validation_steps=1)
history_cnn = best_model.fit(X_train, y_train, validation_data=(X_test,y_test), epochs=20, batch_size=32, shuffle=True, verbose=1, validation_steps=1)
try:
	model_dir='trained_modelB/'
	os.mkdir(model_dir);
except FileExistsError:
	pass;
best_model.save(model_dir+'cnn.h5')
np.savez(model_dir+'training_histories.npz', [ history.history for history in [ history_cnn ]])

