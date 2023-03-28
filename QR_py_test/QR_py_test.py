#https://learn.microsoft.com/ru-RU/visualstudio/python/working-with-c-cpp-python-in-visual-studio?view=vs-2022

#В Обозревателе решений щелкните правой кнопкой мыши узел Ссылки в вашем проекте Python, 
#а затем выберите команду Добавить ссылку.

#В открывшемся диалоговом окне перейдите на вкладку Проекты, выберите проект qr_module
#и нажмите кнопку OK.

#print("Hello World!")

from typing import Iterable
import numpy as np
from qr_module_alpha import get_matrixes
from matplotlib import pyplot as plt

#const data_el data_el01 = {
#	2, "Hello World!", 12
#};
#//const data_el data_el01 = {
#//	2, "30.12.2022 Happy New Year!", 28
#//};
#const data_el data_el02 = {
#	1, " $HJHGHBJ", 9
#};

res_tuple = \
    get_matrixes(
    2, "M", 
    [(2, "Hello World!"), (1, "")],
	[(2, "Hello World!"), (1, "05.01.2023")]
    )

#print(res_tuple)

print("Code:", res_tuple[0])
if not res_tuple[0]:
    matrixes_list = res_tuple[1]

    matrix1 = matrixes_list[1]
    matrix1 = np.frombuffer(matrix1["matrix"], dtype='<u2').reshape(matrix1["side"], matrix1["side"])
    matrix1 = np.where(matrix1 == 65535, 0, matrix1)
    plt.imshow(
        matrix1,
        interpolation='nearest')
    plt.show()

    for i, el in enumerate(matrixes_list):
        if i == 1:
            continue;
        print("score:", el["score"])
        print("side:", el["side"])
        #https://stackoverflow.com/questions/28430904/set-numpy-array-elements-to-zero-if-they-are-above-a-specific-threshold
        #matrix = matrix[matrix == 65535] = 0
        
        matrix = np.frombuffer(el["matrix"], dtype='<u2').reshape(el["side"], el["side"])
        #print(matrix)
        plt.imshow(
            matrix,
            interpolation='nearest')
        plt.show()
        #if i == 0:
        #   break

    #el = matrixes_list[2]
    #print(el["matrix"])
    #print(np.frombuffer(el["matrix"], dtype='<u2').reshape(el["side"], el["side"]))
else:
    print("Error message:", res_tuple[1])

##https://stackoverflow.com/questions/30167538/convert-a-numpy-ndarray-to-stringor-bytes-and-convert-it-back-to-numpy-ndarray
##https://numpy.org/doc/stable/reference/arrays.dtypes.html
##https://numpy.org/doc/stable/reference/generated/numpy.ndarray.tobytes.html#numpy.ndarray.tobytes
##https://numpy.org/doc/stable/reference/arrays.dtypes.html#arrays-dtypes-constructing

##dt = np.dtype('>i2')
#arr = np.array([1, 2, 3, 4, 5, 6], dtype='<u2')
## u2 = unsigned int 2 bytes
#ts = arr.tobytes()
#print(ts)

##print(np.frombuffer(ts, dtype='<u2'))
