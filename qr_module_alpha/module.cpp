//https://learn.microsoft.com/ru-RU/visualstudio/python/working-with-c-cpp-python-in-visual-studio?view=vs-2022
//https://docs.python.org/3/extending/extending.html
//https://python.readthedocs.io/en/stable/c-api/list.html
//https://python.readthedocs.io/en/stable/c-api/tuple.html
//https://python.readthedocs.io/en/stable/c-api/long.html
//https://python.readthedocs.io/en/stable/c-api/concrete.html#fundamental-objects
//https://docs.python.org/3/c-api/unicode.html#PyUnicode_AsUTF8String
//https://docs.python.org/3/c-api/arg.html#c.PyArg_ParseTuple
//https://numpy.org/doc/stable/reference/c-api/index.html
//https://www.programmersought.com/article/53484648943/
//https://spec-zone.ru/numpy~1.10/c-api.types-and-structures#new-python-types-defined
//https://stackoverflow.com/questions/30357115/pyarray-simplenewfromdata-example
//https://docs.python.org/3/c-api/arg.html#c.Py_BuildValue
//https://docs.python.org/3/extending/extending.html?highlight=py_buildvalue

#include <Windows.h>

#define PY_SSIZE_T_CLEAN
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <Python.h>
#include <numpy/arrayobject.h>

#include "QRCode.h"

class ModuleError
{
private:
    const int code;
    const char* err_msg;
public:
    ModuleError(const int code, const char* err_msg) : code(code), err_msg(err_msg) {}
    const char* get_err_msg() const
    {
        return err_msg;
    }
    int get_code() const
    {
        return code;
    }
};

PyObject* get_matrixes(PyObject* /* unused module reference */, PyObject* o_in) {
    PyObject* res_tuple;

    try
    {
        if (!PyTuple_Check(o_in)) throw ModuleError(1, "Input data is NOT tuple");
        if (PyTuple_Size(o_in) != 4) throw ModuleError(11, "Input data length is not 4");

        //int ver;
        //const char* err_corr_level_raw;
        ////https://docs.python.org/3/extending/extending.html?highlight=py_buildvalue
        //const int ok = PyArg_ParseTuple(o_in, "is[][]", 
        //    &ver, &err_corr_level_raw, &s, &size);

        //std::cout << "ok " << ok << std::endl;
        //return Py_BuildValue("(is)", 666, "Sosat\' (Test message)");

        //std::cout << "typeid(o_in).name() " << typeid(o_in).name() << std::endl;

        PyObject* ver_obj = PyTuple_GetItem(o_in, 0);
        if (!PyLong_Check(ver_obj)) throw ModuleError(2, "First argument (ver) is MOT an integer");
        const int ver = static_cast<int>(PyLong_AsSsize_t(ver_obj));

        PyObject* err_corr_level_obj = PyTuple_GetItem(o_in, 1);
        if (!PyUnicode_Check(err_corr_level_obj)) throw ModuleError(3, "Second argument (err_corr_level) is not string");
        ErrCorrLevels err_corr_level;
        const char* err_corr_level_raw = reinterpret_cast<const char*>(PyUnicode_1BYTE_DATA(err_corr_level_obj));
        switch (err_corr_level_raw[0]) //L, M, Q, H
        {
        case 'L':
            err_corr_level = L;
            break;
        case 'M':
            err_corr_level = M;
            break;
        case 'Q':
            err_corr_level = Q;
            break;
        case 'H':
            err_corr_level = H;
            break;
        default:
            throw ModuleError(35, "Second argument (err_corr_level) is not correct");
        }

        PyObject* data_list_1 = PyTuple_GetItem(o_in, 2);
        if (!PyList_Check(data_list_1)) throw ModuleError(4, "Third argument data_list_1 is NOT a list");
        const int data_count_1 = static_cast<int>(PyList_Size(data_list_1));
        data_el* data_1 = new data_el[data_count_1];
        for (int i = 0; i < data_count_1; i++)
        {
            PyObject* py_tuple = PyList_GetItem(data_list_1, i);
            if (!PyTuple_Check(py_tuple)) throw ModuleError(5, "data_list_1[i] is NOT a tuple");
            if (PyTuple_Size(py_tuple) != 2) throw ModuleError(6, "data_list_1[i] length is NOT 2");

            PyObject* mode_obj = PyTuple_GetItem(py_tuple, 0);
            if (!PyLong_Check(mode_obj)) throw ModuleError(7, "data_list_1[i][0] (\"mode\" argument) is NOT an integer");
            const int mode = static_cast<int>(PyLong_AsSsize_t(mode_obj));

            PyObject* val_obj = PyTuple_GetItem(py_tuple, 1);
            if (!PyUnicode_Check(val_obj)) throw ModuleError(8, "data_list_1[i][1] (\"val\" argument) is NOT a string");
            const char* val = reinterpret_cast<const char*>(PyUnicode_1BYTE_DATA(val_obj));
            const int val_len = static_cast<int>(PyUnicode_GET_LENGTH(val_obj));

            data_1[i] = {
                mode,
                val,
                val_len
            };
        }

        PyObject* data_list_2 = PyTuple_GetItem(o_in, 3);
        if (!PyList_Check(data_list_2)) throw ModuleError(9, "Third argument data_list_2 is NOT a list");
        const int data_count_2 = static_cast<int>(PyList_Size(data_list_2));
        data_el* data_2 = new data_el[data_count_2];
        for (int i = 0; i < data_count_2; i++)
        {
            PyObject* py_tuple = PyList_GetItem(data_list_2, i);
            if (!PyTuple_Check(py_tuple)) throw ModuleError(10, "data_list_2[i] is NOT a tuple");
            if (PyTuple_Size(py_tuple) != 2) throw ModuleError(11, "data_list_2[i] length is NOT 2");

            PyObject* mode_obj = PyTuple_GetItem(py_tuple, 0);
            if (!PyLong_Check(mode_obj)) throw ModuleError(12, "data_list_2[i][0] (\"mode\" argument) is NOT an integer");
            const int mode = static_cast<int>(PyLong_AsSsize_t(mode_obj));

            PyObject* val_obj = PyTuple_GetItem(py_tuple, 1);
            if (!PyUnicode_Check(val_obj)) throw ModuleError(13, "data_list_2[i][1] (\"val\" argument) is NOT a string");
            const char* val = reinterpret_cast<const char*>(PyUnicode_1BYTE_DATA(val_obj));
            const int val_len = static_cast<int>(PyUnicode_GET_LENGTH(val_obj));

            data_2[i] = {
                mode,
                val,
                val_len
            };
        }

        QRcode qr_code(ver, err_corr_level);

        PyObject* matrix_arr[10];

        QRcode qr_code_raw(qr_code);
        Matrix mat1 = qr_code_raw.put_data_raw(data_1, data_count_1);
        matrix_arr[0] = 
            Py_BuildValue(
				"{s:y#s:is:i}", "matrix", mat1.get_arr(), mat1.get_arr_len() * TYPE_SIZE, 
				"score", 0,
                "side", mat1.get_m()
            );

        QRcode qr_code_bits_order_matrix(qr_code);
        Matrix mat2 = qr_code_bits_order_matrix.get_bits_order_matrix(data_1, data_count_1);
        matrix_arr[1] = 
            Py_BuildValue(
				"{s:y#s:is:i}", "matrix", mat2.get_arr(), mat2.get_arr_len() * TYPE_SIZE, 
				"score", 0,
                "side", mat2.get_m()
			);

        //for (int i = 0; i < mat2.get_m(); i++)
        //{
        //    for (int j = 0; j < mat2.get_n(); j++)
        //        std::cout << mat2.operator()(i, j) << " ";
        //    std::cout << std::endl;
        //}

    	qr_code.put_data(data_2, data_count_2);
        for (int i = 0; i < 8; i++)
        {
            QRcode qr_code_fin(qr_code);
            Matrix mat = qr_code_fin.make_QR_code(i, false);
            //PyObject* penalty_score_obj = PyLong_FromLong(qr_code_fin.obtain_penalty_score());
            //if (PyList_SetItem(score_list, i, penalty_score_obj) == -1) throw ModuleError(14, "Unable to set \"score_list\" element");
            matrix_arr[2 + i] = Py_BuildValue("{s:y#s:is:i}", "matrix",
                mat.get_arr(), mat.get_arr_len() * TYPE_SIZE,
                "score", qr_code_fin.obtain_penalty_score(),
				"side", mat.get_m()
            );
        }

        delete[] data_1;
        delete[] data_2;

        //return Py_BuildValue("(is)", 666, "Sosat\' (Test message)");

        //int k = 1;
        //for (int i = 0; i < matrix_arr[k]->get_m(); i++)
        //{
        //    for (int j = 0; j < matrix_arr[k]->get_n(); j++)
        //        std::cout << matrix_arr[k]->operator()(i, j) << " ";
        //    std::cout << std::endl;
        //}

        //constexpr int arr_len = 6;
        //uint16 arr[arr_len]
        //{
        //    1, 2, 3, 4, 5, 6
        //};
        //return Py_BuildValue("(iy#)", 666, arr, arr_len * TYPE_SIZE);

        PyObject* matrixes_list = PyList_New(10);
        for (int i = 0; i < 10; i++)
        {
            //const npy_intp* dims = new npy_intp[2]{ matrix_arr[i]->get_m(), matrix_arr[i]->get_n() };
            //PyObject* mat_obj = PyArray_SimpleNewFromData(2, dims, NPY_UINT16, matrix_arr[i]->matrix);
            if (PyList_SetItem(matrixes_list, i, matrix_arr[i]/*mat_obj*/) == -1) throw  ModuleError(15, "Unable to set \"matrixes_list\" element");
            //delete[] dims;
        }


        res_tuple = PyTuple_Pack(2, Py_BuildValue("i", 0), matrixes_list);
        //res_tuple = Py_BuildValue("il", 0, )
        //if (!res_tuple) throw  ModuleError(16, "Unable to create result tuple");
        //return Py_BuildValue("(is)", 666, "Sosat\' (Test message)");

    } catch (ModuleError& err)
    {
        res_tuple = Py_BuildValue("(is)", err.get_code(), err.get_err_msg());
        return res_tuple;
    }
    catch (NotEnoughSize&)
    {
        res_tuple = Py_BuildValue("(is)", 17, "Not enough qr code size");
        return res_tuple;
    }
    catch (Error& /*err*/)
    {
        res_tuple = Py_BuildValue("(is)", 18, "One of QRcode class error");//err.getErrMsg());
        return res_tuple;
    }
    catch (...)
    {
        res_tuple = Py_BuildValue("(is)", 19, "Unhandled error");
        return res_tuple;
    }

    return res_tuple;
}

static PyMethodDef qr_module_alpha_methods[] = {
    // The first property is the name exposed to Python, fast_tanh
    // The second is the C++ function with the implementation
    // METH_O means it takes a single PyObject argument
    { "get_matrixes", static_cast<PyCFunction>(get_matrixes), METH_VARARGS/*METH_O*/, nullptr },

    // Terminate the array with an object containing nulls.
    { nullptr, nullptr, 0, nullptr }
};

static PyModuleDef qr_module_alpha = {
    PyModuleDef_HEAD_INIT,
    "qr_module_alpha",                        // Module name to use with Python import statements
    "Provides qr C++ functions",  // Module description
    0,
    qr_module_alpha_methods                   // Structure that defines the methods of the module
};

PyMODINIT_FUNC PyInit_qr_module_alpha() {
    return PyModule_Create(&qr_module_alpha);
}