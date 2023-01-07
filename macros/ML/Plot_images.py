import sys, os
import numpy as np
# define the function
import matplotlib.pyplot as plt
import matplotlib as mpl


outdir = 'images_out/'
image = np.load(outdir+'allimages.npz')['arr_0']
print(image)
print('We have {} events'.format(len(image)))
print(image.shape)
logscale = False
logscale = dict(norm=mpl.colors.LogNorm()) if logscale else {}

def plot_image_3config(image,evt):
	fig, axes = plt.subplots(1,3, figsize=(12,3))
	for iax, ax in enumerate(axes):
 	   im = ax.imshow(image[iax,evt],**logscale)
 	   plt.colorbar(im, ax=ax)
 
	plt.show()

def plot_image_events(image):
	fig, axes = plt.subplots(1,3, figsize=(12,3))
	for iax, ax in enumerate(axes):
 	   im = ax.imshow(image[1,iax],**logscale)
 	   plt.colorbar(im, ax=ax)
 
	plt.show()

plot_image_3config(image,2)
plot_image_events(image)