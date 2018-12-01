from scipy.io.wavfile import read
from scipy.interpolate import interp1d
import numpy as np
# import matplotlib.pyplot as plt
import sys
import os

def filename_without_ext(path_to_file): return os.path.splitext(path_to_file)[0]

divFactor = int(sys.argv[2])
take = int(sys.argv[3])

a= read(sys.argv[1])

name = filename_without_ext(sys.argv[1])
Name = name.upper()

arr = map (lambda a : a[0],np.array(a[1],dtype=float))
# arr = arr[::4]
arr = np.array(arr)
arr = np.nanmean(np.pad(arr.astype(float), ( 0, 0 if arr.size % divFactor == 0 else divFactor - arr.size % divFactor ), mode='constant', constant_values=np.NaN).reshape(-1, divFactor), axis=1)

mini= min(arr)
maxi = max(arr)
m = interp1d([mini,maxi],[0,255])

new = map (lambda a : int(m(a)), arr)[:take]

strr = map (lambda a : format(a, '#04x'),new)

# print strr

f1=open(name+'.c', 'w+')
f1.write('#include <avr/pgmspace.h>\n\nconst uint8_t  PROGMEM '+name+'_wav[] = {')
for x in strr:
   f1.write(x+', ')
f1.write('};\n\nunsigned int '+name+'_wav_len ='+str(len(strr))+';') 
f1.close()

f2=open(name+'.h', 'w+')
f2.write('#ifndef '+Name+'_H\n#define '+Name+'_H\n\nextern const PROGMEM uint8_t '+name+'_wav[];\n\nextern unsigned int '+name+'_wav_len;\n\n#endif')
f2.close()

# plt.plot(new)
# plt.ylabel('some numbers')
# plt.show()








print name+'\t'+str(len(strr)) +'\t'+ str(len(arr))

# plt.plot(new)
# plt.ylabel('some numbers')
# plt.show()