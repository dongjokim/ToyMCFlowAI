#./ToyMCFlowAI tree_toymcflowAI_bg0NUE0.root 1000 1245 0. 0
seq 1000 1003 | xargs -n1 -P4 -I{} ./ToyMCFlowAI outputs/tree_toymcflowAI_bg0NUE0_{}.root 5000 {} 0. 0

