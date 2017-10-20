#include <Python.h>
#include <stdio.h>
#include "jetsonGPIO.h"

// Write the docstrings for the module
static char module_docstring[] = 
    "This module provides an API for Jetson TX1 GPIO.";
static char gpioExport_docstring[] =
    "Export the given gpio to userspace.\nReturn: Success = 0; otherwise open file error";
static char gpioUnexport_docstring[] =
    "Unexport the given gpio from userspace.\nReturn: Success = 0; otherwise open file error";
static char gpioSetDirection_docstring[] = 
    "Set the direction of the GPIO pin.\nReturn: Success = 0; otherwise open file error";
static char gpioSetValue_docstring[] =
    "Set the value of the GPIO pin to 1 or 0.\nReturn: Success = 0; otherwise open file error";
static char gpioGetValue_docstring[] = 
    "Get the value of the requested GPIO pin; value return is 0 or 1\nReturn: Success = 0; otherwise open file error";
static char gpioSetEdge_docstring[] = 
    "Set the edge of the GPIO pin\nValid edges: 'none' 'rising' 'falling' 'both'\n\nReturn: Success = 0; otherwise open file error";
static char gpioOpen_docstring[] =
    "Open the given pin for reading\nReturns the file descriptor of the named pin";
static char gpioClose_docstring[] =
    "Close the given file descriptor";
static char gpioActiveLow_docstring[] = 
    "Set the active_low attribute of the GPIO pin to 1 or 0\nReturn: Success = 0; otherwise open file error";

// Declare member methods
static PyObject *jetsonGPIO_gpioExport (PyObject *self, PyObject *args);
static PyObject *jetsonGPIO_gpioUnexport (PyObject *self, PyObject *args);
static PyObject *jetsonGPIO_gpioSetDirection (PyObject *self, PyObject *args);
static PyObject *jetsonGPIO_gpioSetValue (PyObject *self, PyObject *args);
static PyObject *jetsonGPIO_gpioGetValue (PyObject *self, PyObject *args);
static PyObject *jetsonGPIO_gpioSetEdge (PyObject *self, PyObject *args);
static PyObject *jetsonGPIO_gpioOpen (PyObject *self, PyObject *args);
static PyObject *jetsonGPIO_gpioClose (PyObject *self, PyObject *args);
static PyObject *jetsonGPIO_gpioActiveLow (PyObject *self, PyObject *args);

// Define member methods of the module
static PyMethodDef module_methods[] = {
    {"gpioExport", jetsonGPIO_gpioExport, METH_VARARGS, gpioExport_docstring},
    {"gpioUnexport", jetsonGPIO_gpioUnexport, METH_VARARGS, gpioUnexport_docstring},
    {"gpioSetDirection", jetsonGPIO_gpioSetDirection, METH_VARARGS, gpioSetDirection_docstring},
    {"gpioSetValue", jetsonGPIO_gpioSetValue, METH_VARARGS, gpioSetValue_docstring},
    {"gpioGetValue", jetsonGPIO_gpioGetValue, METH_VARARGS, gpioGetValue_docstring},
    {"gpioSetEdge", jetsonGPIO_gpioSetEdge, METH_VARARGS, gpioSetEdge_docstring},
    {"gpioOpen", jetsonGPIO_gpioOpen, METH_VARARGS, gpioOpen_docstring},
    {"gpioClose", jetsonGPIO_gpioClose, METH_VARARGS, gpioClose_docstring},
    {"gpioActiveLow", jetsonGPIO_gpioActiveLow, METH_VARARGS, gpioActiveLow_docstring},
    {NULL, NULL, 0, NULL}
};

// START NEW CODE
static struct PyModuleDef moduleStruct =
{
    PyModuleDef_HEAD_INIT,
    "_jetsonGPIO",      /* name of module */
    module_docstring,   /* module documentation, may be NULL */
    -1,                 /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    module_methods
};

PyMODINIT_FUNC PyInit__jetsonGPIO (void)
{
    return PyModule_Create(&moduleStruct);
}
// END NEW CODE

// START OLD CODE
// Define the init function
// PyMODINIT_FUNC init_jetsonGPIO (void)
// {
//     PyObject *m = Py_InitModule ("_jetsonGPIO", module_methods, module_docstring);
//     if (m == NULL)
//         return;
    
// }
// END OLD CODE

static PyObject 
*jetsonGPIO_gpioExport (PyObject *self, PyObject *args)
{
    int pin, value;

    /* Parse the input tuple */
    if (!PyArg_ParseTuple (args, "i", &pin))
        return NULL;
    
    /* Get the GPIO value of the given pin */
    printf("Get PIO from pin\n");
    jetsonGPIO gpio = getGPIOFromInt (pin);
    printf("Got PIO from pin\n");
    printf(gpio ? "true" : "false");
    if (gpio == -1) {
        printf("Erroring out.\n");
        PyErr_SetString (PyExc_ValueError,
                         "Invalid pin number.");
        return NULL;
    }

    /* TODO: Make sure this doesn't error */
    /* Export the given pin and check return value */
    printf("Exporting");
    value = gpioExport (gpio);
    printf("Exported");

    /* Check the return value of the export function */
    if (value != 0) {
        PyErr_SetString (PyExc_RuntimeError,
                         "Error opening pin file.");
        return NULL;
    }

    /* Build the output tuple */
    PyObject *ret = Py_BuildValue ("i", value);
    return ret;
}

static PyObject 
*jetsonGPIO_gpioUnexport (PyObject *self, PyObject *args)
{
    int pin, value;

    /* Parse the input tuple */
    if (!PyArg_ParseTuple (args, "i", &pin))
        return NULL;
    
    /* Get the GPIO value of the given pin */
    jetsonGPIO gpio = getGPIOFromInt (pin);

    if (!gpio) {
        PyErr_SetString (PyExc_ValueError,
                         "Invalid pin number.");
        return NULL;
    }

    /* TODO: Make sure this doesn't error */
    /* Export the given pin and check return value */
    value = gpioUnexport (gpio);

    /* Check the return value of the export function */
    if (value != 0) {
        PyErr_SetString (PyExc_RuntimeError,
                         "Error opening pin file.");
        return NULL;
    }

    /* Build the output tuple */
    PyObject *ret = Py_BuildValue ("i", value);
    return ret;
}

static PyObject
*jetsonGPIO_gpioSetDirection (PyObject *self, PyObject *args)
{
    int pin, value;
    unsigned int out_flag;

    /* Parse the input tuple */
    if (!PyArg_ParseTuple (args, "iI", &pin, &out_flag))
        return NULL;
    
    /* Get the GPIO value of the given pin */
    jetsonGPIO gpio = getGPIOFromInt (pin);

    if (!gpio) {
        PyErr_SetString (PyExc_ValueError,
                         "Invalid pin number.");
        return NULL;
    }

    /* TODO: make sure this doesn't error */
    /* Set the direction and get return value */
    value = gpioSetDirection (gpio, out_flag);

    /* Check the return value of the setDirection function */
    if (value != 0) {
        PyErr_SetString (PyExc_RuntimeError,
                         "Error opening pin file.");
        return NULL;
    }

    /* Build the output tuple */
    PyObject *ret = Py_BuildValue ("i", value);
    return ret;
}

static PyObject
*jetsonGPIO_gpioSetValue (PyObject *self, PyObject *args)
{
    int pin, value;
    unsigned int pin_setting;

    /* Parse the input tuple */
    if (!PyArg_ParseTuple (args, "iI", &pin, &pin_setting))
        return NULL;
    
    /* Get the GPIO value of the given pin */
    jetsonGPIO gpio = getGPIOFromInt (pin);

    if (!gpio) {
        PyErr_SetString (PyExc_ValueError,
                         "Invalid pin number.");
        return NULL;
    }

    /* TODO: make sure this doesn't error */
    /* Set the pin value and get return value */
    value = gpioSetValue (gpio, pin_setting);

    /* Check the return value of the SetValue function */
    if (value != 0) {
        PyErr_SetString (PyExc_RuntimeError,
                         "Error opening pin file.");
        return NULL;
    }

    /* Build the output tuple */
    PyObject *ret = Py_BuildValue ("i", value);
    return ret;
}

static PyObject
*jetsonGPIO_gpioGetValue (PyObject *self, PyObject *args)
{
    int pin, value;
    unsigned int *pin_setting;

    /* Parse the input tuple */
    if (!PyArg_ParseTuple (args, "i", &pin))
        return NULL;
    
    /* Get the GPIO value of the given pin */
    jetsonGPIO gpio = getGPIOFromInt (pin);

    if (!gpio) {
        PyErr_SetString (PyExc_ValueError,
                         "Invalid pin number.");
        return NULL;
    }

    /* TODO: make sure this doesn't error */
    /* Get pin value in pin_setting and get return value */
    value = gpioGetValue (gpio, pin_setting);

    /* Check the return value of the GetValue function */
    if (value != 0) {
        PyErr_SetString (PyExc_RuntimeError,
                         "Error opening pin file.");
        return NULL;
    }

    /* Build the output tuple */
    PyObject *ret = Py_BuildValue ("i", *pin_setting);
    return ret;
}

static PyObject
*jetsonGPIO_gpioSetEdge (PyObject *self, PyObject *args)
{
    int pin, value;
    char *edge;

    /* Parse the input tuple */
    if (!PyArg_ParseTuple (args, "is", &pin, &edge))
    return NULL;

    /* Get the GPIO value of the given pin */
    jetsonGPIO gpio = getGPIOFromInt (pin);

    if (!gpio) {
        PyErr_SetString (PyExc_ValueError,
                         "Invalid pin number.");
        return NULL;
    }

    /* TODO: make sure this doesn't error */
    /* Set the edge and get return value */
    value = gpioSetEdge (gpio, edge);

    /* Check the return value of the setDirection function */
    if (value != 0) {
        PyErr_SetString (PyExc_RuntimeError,
                        "Error opening pin file.");
        return NULL;
    }

    /* Build the output tuple */
    PyObject *ret = Py_BuildValue ("i", value);
    return ret;
}

static PyObject
*jetsonGPIO_gpioOpen (PyObject *self, PyObject *args)
{
    int pin, fileDescriptor;

    /* Parse the input tuple */
    if (!PyArg_ParseTuple (args, "i", &pin))
        return NULL;
    
    /* Get the GPIO value of the given pin */
    jetsonGPIO gpio = getGPIOFromInt (pin);

    if (!gpio) {
        PyErr_SetString (PyExc_ValueError,
                         "Invalid pin number.");
        return NULL;
    }

    /* TODO: make sure this doesn't error */
    /* Open the pin and get the file descriptor */
    fileDescriptor = gpioOpen (gpio);

    if (!fileDescriptor) {
        PyErr_SetString (PyExc_RuntimeError,
                         "Error getting file descriptor");
        return NULL;
    }

    /* Build the output tuple */
    PyObject *ret = Py_BuildValue ("i", fileDescriptor);
    return ret;
}

static PyObject
*jetsonGPIO_gpioClose (PyObject *self, PyObject *args)
{
    int fileDescriptor, value;

    /* Parse the input tuple */
    if (!PyArg_ParseTuple (args, "i", &fileDescriptor))
        return NULL;

    /* TODO: make sure this doesn't error */
    /* Close the pin and get the file descriptor */
    value = gpioClose (fileDescriptor);

    if (!value) {
        PyErr_SetString (PyExc_RuntimeError,
                         "Error getting file descriptor from close.");
        return NULL;
    }

    /* Build the output tuple */
    PyObject *ret = Py_BuildValue ("i", value);
    return ret;
}

static PyObject 
*jetsonGPIO_gpioActiveLow (PyObject *self, PyObject *args)
{
    int pin, value;
    unsigned int active_low_setting;

    /* Parse the input tuple */
    if (!PyArg_ParseTuple (args, "iI", &pin, &active_low_setting))
        return NULL;

    /* Get the GPIO value of the given pin */
    jetsonGPIO gpio = getGPIOFromInt (pin);

    if (!gpio) {
        PyErr_SetString (PyExc_ValueError,
                         "Invalid pin number.");
        return NULL;
    }
    
    /* TODO: Make sure this doesn't error */
    /* Set the active_low attribute to active_low_setting */
    value = gpioActiveLow (gpio, active_low_setting);

    if (value != 0) {
        PyErr_SetString (PyExc_RuntimeError,
                         "Error opening pin file.");
        return NULL;
    }

    /* Build the output tuple */
    PyObject *ret = Py_BuildValue ("i", value);
    return ret;
}