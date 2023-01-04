import uproot3
import ROOT
import pandas as pd
import numpy as np
pd.options.plotting.backend = "plotly"
import plotly.express as px
from plotly.subplots import make_subplots
import plotly.graph_objs as go
import matplotlib.pyplot as plt
import seaborn as sns
df = px.data.tips()

mux = pd.MultiIndex.from_product([[],[]],names=["event","Entry"]);
dataPhi = pd.DataFrame(columns=mux);

tree = uproot3.open("../test.root")['vTree']
print(tree.keys())
#tree.arrays(["phi", "eta", "pt"])
df = tree.pandas.df()
print(df)


xtitle = "$\\eta$"
ytitle = "$\\varphi (\\mathrm{rad})$"

for i in range(0,10):
	myevent = df.loc[df['event'] == i]
	hphi = myevent['phi'].to_numpy()
	N = hphi.size
	t = np.arange(N)
	f = np.cos(2*np.pi * t/N)
	ft = np.fft.fft(f)
	print(ft)
	#ic = myevent['icent']
	print(i, hphi)
	#fig = px.histogram(myevent, x="phi", nbins=200)
	plt.scatter(x=myevent['eta'], y=myevent['phi'],s=5*myevent['pt'],vmin=0, vmax=10)
	plt.xlabel(xtitle)
	plt.ylabel(ytitle)
	#sns.kdeplot(data = myevent, x="eta", y="phi", cmap="Reds", shade=True)
	plt.show()
	#dataPhi[i,"phi"] = hphi.to_numpy();

#print(dataPhi)