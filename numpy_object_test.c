/**
 *  Access a numpy ndarray of str objects
 */
#include <Python.h>
#include <numpy/arrayobject.h>

#define DLL_PUBLIC __attribute__((visibility("default")))

DLL_PUBLIC
void test_numpy_array(PyObject *obj) {
    PyArrayObject * in_array = reinterpret_cast<PyArrayObject *>(obj);
    int ndim = PyArray_NDIM(in_array);
    const long * dims = PyArray_SHAPE(in_array);

    char* item_ptr = (char *)PyArray_GETPTR1(in_array, 0);
    PyObject* str0 = PyArray_GETITEM(in_array, item_ptr);
    const size_t length = PyString_Size(str0);
    const char *p = PyString_AsString(str0);
}

DLL_PUBLIC
static PyObject * test_pyobject(PyObject *self, PyObject * args) {
    PyArrayObject *in_array;
    if (!PyArg_ParseTuple(args, "O!", &PyArray_Type, &in_array)) {
        Py_RETURN_NONE;
    }

    int ndim = PyArray_NDIM(in_array);
    const long * dims = PyArray_SHAPE(in_array);

    char* item_ptr = (char *)PyArray_GETPTR1(in_array, 0);
    PyObject* str0 = PyArray_GETITEM(in_array, item_ptr);
    const size_t length = PyString_Size(str0);
    const char *p = PyString_AsString(str0);
    Py_RETURN_NONE;
}

static PyMethodDef testMethods[] = {
    {"test_pyobject",  test_pyobject, METH_VARARGS, ""},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC
initconcat(void)
{
    (void) Py_InitModule("concat", testMethods);
    import_array();
}
