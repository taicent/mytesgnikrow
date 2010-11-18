__all__ = ['argsort','argsort_and_cut']

from scipy.weave import inline
from numpy import zeros, arange, array

from .. import dtype_numpy_to_C

def argsort(a):
    """Take a 1D array of type '[u]int<8|16|32|64>' or 'float<32|64>', and return a 1D array of indices that sorts the former in ascending order.
    """
    if a.ndim != 1:
        raise TypeError("Input a is not 1D")
    
    try:
        dtype = dtype_numpy_to_C[a.dtype.name]
    except KeyError:
        raise TypeError("Input a has unknown dtype")
    
    id = zeros(len(a),'int')
    
    code = "typedef "+dtype+" dtype;"
    code += """
        argsort<dtype>((int)Na[0],(dtype *)a,(int *)id);
        return_val = 1;
    """
    inline(code, arg_names=['a','id'], headers=['<argsort.h>'], 
        libraries=['sdcpp', 'cblas_ext', 'cephesd', 'plat_det'])
    
    return id
    
def argsort_and_cut(a):
    """Find argsort and 'list of cuts' of a 1D 'double' array

    Input:
        a: a 1D 'double' array
    Output:
        id: an index array that sorts a in ascending orders
        B: an array of thresholds that cuts a properly
    """
    if a.ndim != 1:
        raise TypeError("Input a is not 1D")
    if a.dtype.name != 'float64':
        raise TypeError("dtype of a is not 'float64'")
        
    bid = argsort(a)
    B = zeros(len(a)+1)
    
    code = """
    	int i, nb, N = Na[0];

    	// compute values for b
    	nb = 1;
    	B[0] = a[bid[0]]-1.0;
    	for( i = 1; i < N; ++i ) if(a[bid[i]] > a[bid[i-1]])
    		B[nb++]= 0.5*(a[bid[i]]+a[bid[i-1]]);
    	B[nb++] = a[bid[N-1]]+1.0;

    	return_val = nb;
    """
    n = inline(code, arg_names=['a','bid','B'])

    return (bid,B[:n])
