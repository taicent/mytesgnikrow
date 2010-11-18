#-------------------------------------------------------------------------------
# Module:      linalg
# Package:     sd.cs
# Purpose:     Linear Algebra subroutines
#
# Author:      minhtri
#
# Created:     25/03/2007
# Copyright:   (c) minhtri 2007
# Licence:     Lesser GPL
#-------------------------------------------------------------------------------
#!/usr/bin/env python

__all__ = ['symm_eig', 'ordered_eigh']

from numpy import dot, where, seterr, sqrt, isfinite, flipud
from numpy.linalg import eigh

def symm_eig(a):
   """Return the eigenvectors and eigenvalues of the symmetric matrix a'*a. If
   a has more columns than rows, then that matrix will be rank-deficient,
   and the non-zero eigenvalues and eigenvectors can be more easily extracted
   from the matrix a*a', from the properties of the SVD:
     if a of shape (m,n) has SVD u*s*v', then:
       a'*a = v*s'*s*v'
       a*a' = u*s*s'*u'
   That is, v contains the eigenvectors of a'*a, with s'*s the eigenvalues,
   according to the eigen-decomposition theorem.
   Now, let s_hat, an array of shape (m,n), be such that s * s_hat = I(m,m)
     and s_hat * s = I(n,n). With that, we can solve for u or v in terms of the
     other:
       v = a'*u*s_hat'
       u = a*v*s_hat
   """
   m, n = a.shape
   if m >= n:
     # just return the eigenvalues and eigenvectors of a'a
     vecs, vals = eigh(dot(a.transpose(), a))
     vecs = where(vecs < 0, 0, vecs)
     return vecs, vals
   else:
     # figure out the eigenvalues and vectors based on aa', which is smaller
     sst_diag, u = eigh(dot(a, a.transpose()))
     # in case due to numerical instabilities we have sst_diag < 0 anywhere,
     # peg them to zero
     sst_diag = where(sst_diag < 0, 0, sst_diag)
     # now get the inverse square root of the diagonal, which will form the
     # main diagonal of s_hat
     err = seterr(divide='ignore', invalid='ignore')
     s_hat_diag = 1/sqrt(sst_diag)
     seterr(**err)
     s_hat_diag = where(isfinite(s_hat_diag), s_hat_diag, 0)
     # s_hat_diag is a list of length m, a'u is (n,m), so we can just use
     # numpy's broadcasting instead of matrix multiplication, and only create
     # the upper mxm block of a'u, since that's all we'll use anyway...
     v = dot(a.transpose(), u[:,:m]) * s_hat_diag
     return sst_diag, v

def ordered_eigh(m):
   """Return eigenvalues and corresponding eigenvectors of the hermetian
   array m, ordered by eigenvalues in decreasing order.
   Note that the numpy.linalg.eigh makes no order guarantees."""
   values, vectors = eigh(m)
   order = flipud(values.argsort())
   return values[order], vectors[:,order]
