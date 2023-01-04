import uproot3
import ROOT
import pandas as pd
import numpy as np
pd.options.plotting.backend = "plotly"
import plotly.express as px
df = px.data.tips()

mux = pd.MultiIndex.from_product([[],[]],names=["event","Entry"]);
dataPhi = pd.DataFrame(columns=mux);


tree = uproot3.open("../test.root")['vTree']

print(tree.keys())

tree.arrays(["phi", "eta", "pt"])
df = tree.pandas.df()
print(df)

for i in range(0,10):
	myevent = df.loc[df['event'] == i]
	hphi = myevent['phi'];
	#ic = myevent['icent']
	print(i, hphi)
	#myevent["phi"].plot(kind = 'hist')
	fig = px.histogram(myevent, x="phi", nbins=200)
	fig.show()
	#dataPhi[i,"phi"] = hphi.to_numpy();

#print(dataPhi)