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
from sklearn import preprocessing
from glob import glob

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

#todo: shared with train model, make a function
outdir = 'images_out/'
for fn in glob(outdir+"images*.npz"):
	io = np.load(fn);
	data = io['arr_0']
	obs = io['arr_1']

	print('We have all {} events and {} true v2 '.format(data.shape[0],obs.shape[0]))
	ndim = 3*32*32
	x = data.reshape(data.shape[0],ndim)
	try:
		X = np.concatenate((X,x));
		y = np.concatenate((y,obs));
	except NameError:
		X = x;
		y = obs;
		
	print("Loaded {} ".format(fn),X.shape);

X = preprocessing.normalize(X,norm='l2'); #L2 normalization scheme
y = y[:, 0] # v2 only

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

