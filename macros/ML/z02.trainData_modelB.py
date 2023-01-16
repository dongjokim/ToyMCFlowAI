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
from keras.layers import Dense, Flatten, Dropout, Activation, Conv2D, MaxPooling2D
from tensorflow.keras import models, layers, utils, backend as K
from plot_mymodel import *
from model_B import *
from glob import glob

outdir = 'images_out/'
for fn in glob(outdir+"images*.npz"):#[:3]:
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

exec(open("model_B.py").read())

n_features = ndim # per event[:,0]

model_cnn = MyModel(n_features)

plot_model(model_cnn, to_file='figs/modelB_plot.png', show_shapes=True, show_layer_names=True)
#visualize_nn(model_cnn)

# Compile model
model_cnn.compile(optimizer='adam', loss=tf.keras.losses.MeanSquaredError());
#epochs=10 to not overfit
#batch_size=32 (PRD)
history_cnn = model_cnn.fit(X, y, validation_split=0.3, epochs=15, batch_size=32, shuffle=False, verbose=1, validation_steps=1)
try:
	model_dir='trained_modelB/'
	os.mkdir(model_dir);
except FileExistsError:
	pass;
model_cnn.save(model_dir+'cnn.h5')
np.savez(model_dir+'training_histories.npz', [ history.history for history in [ history_cnn ]])

