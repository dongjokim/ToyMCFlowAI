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
for fn in glob(outdir+"images*.npz"):
	io = np.load(fn);
	data = io['arr_0']  # (3000, 32, 32)
	obs = io['arr_1']
	#print('We have all {} events and {} true v2 '.format(len(data),len(dataTrue)))
	# allimages (3000, 32, 32)
	# need to do X[ievt]=3x32x32 per event (total 1000evt)
	# the data coming out of previous commands is a list of 2D arrays. We want a 3D np array (n_events, xpixels, ypixels)
	ndim = 3*32*32
	x = data.reshape(1000,ndim)
	try:
		X = np.concatenate((X,x));
		y = np.concatenate((y,obs));
	except NameError:
		X = x;
		y = obs;
		
	print("Loaded {} ".format(fn),X.shape);

X = preprocessing.normalize(X,norm='l2'); #L2 normalization scheme
y = y[:, 0] # v2 only

print(X.shape, y.shape)

exec(open("model_B.py").read())

n_features = ndim # per event

model_cnn = MyModel(n_features)

plot_model(model_cnn, to_file='figs/modelB_plot.png', show_shapes=True, show_layer_names=True)
#visualize_nn(model_cnn)

# Compile model
model_cnn.compile(optimizer='adam', loss=tf.keras.losses.MeanSquaredError());
#epochs=10 to not overfit
#batch_size=32 (PRD)
history_cnn = model_cnn.fit(X, y, validation_split=0.3, epochs=10, batch_size=32, shuffle=True, verbose=1)
model_dir='trained_modelB/'
if not os.path.isdir(model_dir): os.system('mkdir '+model_dir)
model_cnn.save(model_dir+'cnn.h5')
np.savez(model_dir+'training_histories.npz', [ history.history for history in [ history_cnn ]])
