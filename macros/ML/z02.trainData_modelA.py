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
from plot_mymodel import *
from model_A import *

outdir = 'images_out/'
data = np.load(outdir+'allimages.npz')['arr_0']
dataTrue = np.load(outdir+'trueimages.npz')['arr_0']
print('We have all {} events and {} true v2 '.format(len(data),len(dataTrue)))

# the data coming out of previous commands is a list of 2D arrays. We want a 3D np array (n_events, xpixels, ypixels)
x_data = np.concatenate((data, dataTrue))
# pad and normalize images
#x_data = list(map(pad_image, x_data))
#x_data = list(map(normalize, x_data))
y_data = np.array([0]*len(data)+[1]*len(dataTrue))

# the data coming out of previous commands is a list of 2D arrays. We want a 3D np array (n_events, xpixels, ypixels)
x_data = np.stack(x_data)

print(x_data.shape, y_data.shape)

# reshape for tensorflow: x_data.shape + (1,) = shortcut for (x_data.shape[0], 16, 22, 1)
x_data = x_data.reshape(x_data.shape + (1,)).astype('float32')
x_data /= 255.

y_data = keras.utils.to_categorical(y_data,2)

print(x_data.shape, y_data.shape)

n_train = 1000
(x_train, x_test) = x_data[:n_train], x_data[n_train:]
(y_train, y_test) = y_data[:n_train], y_data[n_train:]

print('We will train+validate on {0} images, leaving {1} for cross-validation'.format(n_train,len(x_data)-n_train))

exec(open("model_A.py").read())

model0, model1, history_mlp,model_cnn = MyModel(x_train, y_train)
plot_model(model_cnn, to_file='figs/modelA_plot.png', show_shapes=True, show_layer_names=True)
#visualize_nn(model_cnn)
# Compile model
model_cnn.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy'])
history_cnn = model_cnn.fit(x_train, y_train, validation_split=0.2, epochs=40, batch_size=100, shuffle=True, verbose=1)
model_dir='trained_modelA/'
if not os.path.isdir(model_dir): os.system('mkdir '+model_dir)
model0.save(model_dir+'logi.h5')
model1.save(model_dir+'mlp.h5')
model_cnn.save(model_dir+'cnn.h5')
np.savez(model_dir+'training_histories.npz', [ history.history for history in [ history_cnn ]])
