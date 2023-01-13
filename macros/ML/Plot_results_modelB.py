import sys, os
import numpy as np
# define the function
import matplotlib.pyplot as plt
import matplotlib as mpl
import keras
# save np.load
np_load_old = np.load
from keras.models import Sequential
from keras.layers import Dense, Flatten, Dropout, Activation, Conv2D, MaxPooling2D
from tensorflow.keras import models, layers, utils, backend as K

def F1(y_true, y_pred):
    precision = Precision(y_true, y_pred)
    recall = Recall(y_true, y_pred)
    return 2*((precision*recall)/(precision+recall+K.epsilon()))

cmap = plt.get_cmap('gray_r')
# modify the default parameters of np.load
np.load = lambda *a,**k: np_load_old(*a, allow_pickle=True, **k)

model_dir='trained_modelB/'

history_cnn = np.load(model_dir+'training_histories.npz')['arr_0']
#print(history_cnn)
model_cnn = keras.models.load_model("trained_modelB/cnn.h5",custom_objects={"F1": F1 })
print(model_cnn)

outdir = 'images_out/'
data = np.load(outdir+'allimages.npz')['arr_0']  # (3000, 32, 32)
dataTrue = np.load(outdir+'flowprop.npz')['arr_0'] # (3000, 4)
print('We have all {} events and {} true v2 '.format(len(data),len(dataTrue)))
# allimages (3000, 32, 32)
# need to do X[ievt]=3x32x32 per event (total 1000evt)
# the data coming out of previous commands is a list of 2D arrays. We want a 3D np array (n_events, xpixels, ypixels)
ndim = 3*32*32
X = data.reshape(1000,ndim)
y = dataTrue[:, 0] # v2 only

n_train = 700;
(x_train, x_test) = X[:n_train], X[n_train:]
(y_train, y_test) = y[:n_train], y[n_train:]

predictions_cnn = model_cnn.predict(x_test)
#print(predictions_cnn.shape);

print(y_test[:5],predictions_cnn[:,0][:5]);

fig,ax = plt.subplots(1,1,figsize=(5,5));
ax.scatter(y_test,predictions_cnn[:,0]);
fig.tight_layout();
fig.savefig('figs/cnn_test_modelB_corr.png',dpi=150);

