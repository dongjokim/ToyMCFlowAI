import sys, os
import numpy as np
# define the function
import matplotlib.pyplot as plt
import matplotlib as mpl


outdir = 'images_out/'
allimages = np.load(outdir+'allimages.npz')['arr_0']
trueimages = np.load(outdir+'trueimages.npz')['arr_0']
print('We have all {} events and {} true v2 '.format(len(allimages),len(trueimages)))

print(allimages.shape)
logscale = False
logscale = dict(norm=mpl.colors.LogNorm()) if logscale else {}



for idx in range(5):
	fig, axes = plt.subplots(1,2, figsize=(12,3))
	for iax, ax in enumerate(axes):
		im = ax.imshow([allimages,trueimages][iax][idx], **logscale)
		plt.colorbar(im, ax=ax)
		#ax.set(title='{} jet: $p_T=${:.0f} GeV'.format(['QCD','top'][iax], [jetpep0,jetpep][iax][idx][0][0]))

	plt.show()


# def plot_image_3config(image,evt):
# 	fig, axes = plt.subplots(1,3, figsize=(12,3))
# 	for iax, ax in enumerate(axes):
#  	   im = ax.imshow(image[iax,evt],**logscale)
#  	   plt.colorbar(im, ax=ax)
 
# 	plt.show()

# def plot_image_events(image):
# 	fig, axes = plt.subplots(1,3, figsize=(12,3))
# 	for iax, ax in enumerate(axes):
#  	   im = ax.imshow(image[1,iax],**logscale)
#  	   plt.colorbar(im, ax=ax)
 
# 	plt.show()

# plot_image_3config(image,2)
# plot_image_events(image)