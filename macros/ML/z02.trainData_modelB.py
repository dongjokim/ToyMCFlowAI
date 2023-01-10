import sys, os
import numpy as np
import matplotlib.pyplot as plt
# define the function
import tensorflow as tf
import shap
from tensorflow.keras.utils import plot_model
import keras
from keras.models import Sequential
from keras.layers import Dense, Flatten, Dropout, Activation, Conv2D, MaxPooling2D
from tensorflow.keras import models, layers, utils, backend as K
from plot_mymodel import *
from model_B import *
def binary_step_activation(x):
    ##return 1 if x>0 else 0 
    return K.switch(x>0, tf.math.divide(x,x), tf.math.multiply(x,0))

# define metrics
def Recall(y_true, y_pred):
    true_positives = K.sum(K.round(K.clip(y_true * y_pred, 0, 1)))
    possible_positives = K.sum(K.round(K.clip(y_true, 0, 1)))
    recall = true_positives / (possible_positives + K.epsilon())
    return recall

def Precision(y_true, y_pred):
    true_positives = K.sum(K.round(K.clip(y_true * y_pred, 0, 1)))
    predicted_positives = K.sum(K.round(K.clip(y_pred, 0, 1)))
    precision = true_positives / (predicted_positives + K.epsilon())
    return precision

def F1(y_true, y_pred):
    precision = Precision(y_true, y_pred)
    recall = Recall(y_true, y_pred)
    return 2*((precision*recall)/(precision+recall+K.epsilon()))

def R2(y, y_hat):
    ss_res =  K.sum(K.square(y - y_hat)) 
    ss_tot = K.sum(K.square(y - K.mean(y))) 
    return ( 1 - ss_res/(ss_tot + K.epsilon()) )

outdir = 'images_out/'
data = np.load(outdir+'allimages.npz')['arr_0']  # (3000, 32, 32)
dataTrue = np.load(outdir+'flowprop.npz')['arr_0'] # (3000, 4)
print('We have all {} events and {} true v2 '.format(len(data),len(dataTrue)))
# allimages (3000, 32, 32)
# need to do X[ievt]=3x32x32 per event (total 1000evt)
# the data coming out of previous commands is a list of 2D arrays. We want a 3D np array (n_events, xpixels, ypixels)
ndim = 3*32*32
X = data.reshape(1000,ndim)
print(X[0])
y = dataTrue[:, 0] # v2 only

print(X.shape, y.shape)

exec(open("model_B.py").read())

n_features = ndim # per event

model_cnn = MyModel(n_features)

plot_model(model_cnn, to_file='figs/model_plot.png', show_shapes=True, show_layer_names=True)
#visualize_nn(model_cnn)

# Compile model
model_cnn.compile(optimizer='adam', loss='binary_crossentropy', metrics=['accuracy',F1])
history_cnn = model_cnn.fit(X, y, validation_split=0.2, epochs=40, batch_size=100, shuffle=True, verbose=1)
model_dir='trained_modelB/'
if not os.path.isdir(model_dir): os.system('mkdir '+model_dir)
model0.save(model_dir+'logi.h5')
model1.save(model_dir+'mlp.h5')
model_cnn.save(model_dir+'cnn.h5')
np.savez(model_dir+'training_histories.npz', [ history.history for history in [history_logi, history_mlp, history_cnn ]])
