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

N = 0;
#todo: shared with train model, make a function
outdir = 'images_out/'
for fn in glob(outdir+"images*.npz"):#[:3]:
	l = np.load(fn);
	data = l['arr_0']
	obs = l['arr_1']

	print('We have {} events and {} true v2 '.format(data.shape[0],obs.shape[0]))
	N += data.shape[0];

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
#y = preprocessing.normalize(y,norm='l1');
y = y[:,0] # v2 only

n_train = int(np.ceil(0.7*N));
(x_train, x_test) = X[:n_train], X[n_train:]
(y_train, y_test) = y[:n_train], y[n_train:]

predictions_cnn = model_cnn.predict(x_test)

print(y_test[:5],"\n",predictions_cnn[:,0][:5]);

fig,ax = plt.subplots(1,1,figsize=(5,5));
ax.scatter(y_test,predictions_cnn[:,0]);
#z,xe,ye = np.histogram2d(y_test,predictions_cnn[:,0],bins=(100,100));#,weights=myevent['pt'])
#m = z.max();
#z[z < 1] = n
#x = 0.5*(xe[:-1]+xe[1:]);
#y = 0.5*(ye[:-1]+ye[1:]);
#ax.contourf(x,y,z,levels=10,norm=mpl.colors.LogNorm(1,m));
fig.tight_layout();
fig.savefig('figs/cnn_test_modelB_corr.png',dpi=150);

