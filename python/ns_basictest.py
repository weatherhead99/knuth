import numpy as np
import libknuth_python

dat = np.random.normal(size=100)

ns = libknuth_python.NestedSamplingOptbins(dat,10,1,1)
ns.init_samples_uniform()

