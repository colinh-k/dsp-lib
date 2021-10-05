# a python script to plot signal files produced by
#  dsp algorithms in C. the signal files should contain
#  a newline-delimited sequence of floating point numbers
import matplotlib.pyplot as plt
import numpy as np
import sys

# def test_dft():
#   samples = [0 for i in range(0,16)]
#   samples[3] = 50
#   samples[4] = -18
#   samples[5] = -9
#   samples[6] = -6
#   samples[7] = -2

#   dft = np.fft.fft(samples)
#   freq = np.fft.fftfreq(samples)
#   inverse = np.fft.ifft(dft)

#   plt_color = 'red'

#   # plt.scatter(range(0, len(dft)), dft, s=10, c=plt_color)
#   plt.grid()

#   plt.xlabel('Sample Number')
#   plt.ylabel('Amplitude')
#   plt.plot(freq, dft.real, freq, dft.imag)
#   # plt.plot(range(0, len(dft)), dft, c=plt_color)

def main():
  if len(sys.argv) < 2:
    print('Usage: ptyhon3 plot.py <filename.sig>')
    exit()

  plt_color = 'black'

  for filename in sys.argv[1:]:
    t = []
    with open(filename, 'r') as f:
      for line in f.readlines():
        # use 'if' in comprehension in case a value is
        #  empty (i.e. not a float)
        data = [float(i) for i in line.split('\n') if i.strip()]
        t += data
    y = [i for i in range(0, len(t))]

    plt.figure()
    plt.scatter(y, t, s=10, c=plt_color)
    plt.grid()
    plt.title(filename)
    plt.xlabel('Sample Number')
    plt.ylabel('Amplitude')
    plt.plot(y, t, c=plt_color)

  plt.show()
  # test_dft()

if __name__ == "__main__":
  main()