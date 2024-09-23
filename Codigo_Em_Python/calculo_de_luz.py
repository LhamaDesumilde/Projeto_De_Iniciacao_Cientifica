def calcula_lum(bin):
  import math
  tensao = 5 - (bin/1023.0)*5.0
  lum = 72.197 * (tensao ** -2.037)
  print(lum)
  return lum