//    Copright (C) 2007, Minh-Tri Pham
//
//    This is a re-implememtation from scratch of Bernd Gaertner's idea at http://www.inf.ethz.ch/personal/gaertner
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <iostream>
#include <list>

#include "SdMinBall.h"

// Functions
// =========
inline double sqr (double r) {return r*r;}

inline double norm2(const SdVector &p)
{
    int i = p.size();
    double err = 0;
    while(--i >= 0) err += sqr(p[i]);
    return err;
}

inline double norm2(const SdVector &p1, const SdVector &p2)
{
    int i = p1.size();
    double err = 0;
    while(--i >= 0) err += sqr(p1[i]-p2[i]);
    return err;
}


// Class Definitions
// =================

// SdMinBall_core
// ----------
// smallest ball with a set of n <= d+1 points *on the boundary*

SdMinBall_core::SdMinBall_core(int dim)
    : d(dim), q0(dim), z(dim+1), f(dim+1), v(dim+1), a(dim+1), c(dim+1), sqr_r(dim+1),
    current_c(c[0])

{
    for (int i=0; i<d+1; ++i) v[i].resize(d);
    for (int i=0; i<d+1; ++i) a[i].resize(d);
    for (int i=0; i<d+1; ++i) c[i].resize(d);
    reset();
}

// access
const SdVector &    SdMinBall_core::center() const { return current_c; }
double              SdMinBall_core::squared_radius() const { return current_sqr_r; }
int                 SdMinBall_core::size() const { return m; }
int                 SdMinBall_core::support_size() const { return s; }
double              SdMinBall_core::excess (const SdVector& p) const { return norm2(p,current_c)-current_sqr_r; }


// modification
void                SdMinBall_core::reset() // generates empty sphere with m=s=0
{
    m = s = 0;
    // we misuse c[0] for the center of the empty sphere
    c[0] = 0;
    current_c = c[0];
    current_sqr_r = -1;
}

bool                SdMinBall_core::push (const SdVector& p)
{
    int i, j;
    double eps = 1e-32;
    if (!m) {
        q0 = p;
        c[0] = q0;
        sqr_r[0] = 0;
    } else {
        // set v_m to Q_m
        v[m] = p-q0;

        // compute the a_{m,i}, i< m
        for (i=1; i<m; ++i) {
            a[m][i] = 0;
            for (j=0; j<d; ++j) a[m][i] += v[i][j] * v[m][j];
            a[m][i] *= (2/z[i]);
        }

        // update v_m to Q_m-\bar{Q}_m
        for (i=1; i<m; ++i) for (j=0; j<d; ++j) v[m][j] -= a[m][i]*v[i][j];

        // compute z_m
        z[m] = norm2(v[m])*2;

        // reject push if z_m too small
        if (z[m]<eps*current_sqr_r) return false;

        // update c, sqr_r
        double e = norm2(p,c[m-1])-sqr_r[m-1];
        f[m]=e/z[m];

        for (i=0; i<d; ++i) c[m][i] = c[m-1][i]+f[m]*v[m][i];
        sqr_r[m] = sqr_r[m-1] + e*f[m]/2;
    }

    current_c = c[m];
    current_sqr_r = sqr_r[m];
    s = ++m;
    return true;
}

void                SdMinBall_core::pop () { --m; }

// checking
double              SdMinBall_core::slack() const
{
    SdVector l(d+1);
    double min_l=0;
    l[0] = 1;
    for (int i=s-1; i>0; --i) {
        l[i] = f[i];
        for (int k=s-1; k>i; --k) l[i]-=a[k][i]*l[k];
        if (l[i] < min_l) min_l = l[i];
        l[0] -= l[i];
    }
    if (l[0] < min_l) min_l = l[0];
    return ( (min_l < 0) ? -min_l : 0);
}


// SdMinBall
// --------
// smallest enclosing ball of a set of n points in dimension d

void        SdMinBall::mtf_mb (It i)
{
    support_end = L.begin();
    if ((B.size())==d+1) return;
    for (It k=L.begin(); k!=i;) {
        It j=k++;
        if (B.excess(*j) > 0) {
            if (B.push(*j)) {
                mtf_mb (j);
                B.pop();
                move_to_front(j);
            }
        }
    }
}

void        SdMinBall::pivot_mb (It i)
{
    It t = ++L.begin();
    mtf_mb (t);
    double max_e, old_sqr_r = -1;
    do {
        It pivot;
        max_e = max_excess (t, i, pivot);
        if (max_e > 0) {
            t = support_end;
            if (t==pivot) ++t;
            old_sqr_r = B.squared_radius();
            B.push (*pivot);
            mtf_mb (support_end);
            B.pop();
            move_to_front (pivot);
        }
    } while ((max_e > 0) && (B.squared_radius() > old_sqr_r));
}

void        SdMinBall::move_to_front (It j)
{
    if (support_end == j) support_end++;
    L.splice (L.begin(), L, j);
}

double      SdMinBall::max_excess (It t, It i, It& pivot) const
{
    const SdVector & c = B.center();
    const double sqr_r = B.squared_radius();
    double e, max_e = 0;
    for (It k=t; k!=i; ++k) {
        e = norm2(*k,c)-sqr_r;
        if (e > max_e) {
            max_e = e;
            pivot = k;
        }
    }
    return max_e;
}

// creates an empty ball
SdMinBall::SdMinBall(int dim) : d(dim), B(dim) {}

// copies p to the internal point set
void        SdMinBall::check_in (const SdVector& p)
{
    assert (d == (int) p.size());
    L.push_back(p);
}


// builds the smallest enclosing ball of the internal point set
void        SdMinBall::build ()
{
    B.reset();
    support_end = L.begin();
    pivot_mb (L.end());
}

// returns center of the ball (undefined if ball is empty)
const SdVector &      SdMinBall::center() const { return B.center(); }

// returns squared_radius of the ball (-1 if ball is empty)
double      SdMinBall::squared_radius () const { return B.squared_radius(); }

// returns size of internal point set
int         SdMinBall::nr_points () const { return L.size(); }

// returns begin- and past-the-end iterators for internal point set
SdMinBall::Cit         SdMinBall::points_begin () const { return L.begin(); }
SdMinBall::Cit         SdMinBall::points_end () const { return L.end(); }

// returns size of support point set; this set has the following properties:
// - there are at most d+1 support points,
// - all support points are on the boundary of the computed ball, and
// - the smallest enclosing ball of the support point set equals the
//   smallest enclosing ball of the internal point set
int         SdMinBall::nr_support_points () const { return B.support_size(); }

// returns begin- and past-the-end iterators for internal point set
SdMinBall::Cit         SdMinBall::support_points_begin () const { return L.begin(); }
SdMinBall::Cit         SdMinBall::support_points_end () const { return support_end; }

// assesses the quality of the computed ball. The return value is the
// maximum squared distance of any support point or point outside the
// ball to the boundary of the ball, divided by the squared radius of
// the ball. If everything went fine, this will be less than e-15 and
// says that the computed ball approximately contains all the internal
// points and has all the support points on the boundary.
//
// The slack parameter that is set by the method says something about
// whether the computed ball is really the *smallest* enclosing ball
// of the support points; if everything went fine, this value will be 0;
// a positive value may indicate that the ball is not smallest possible,
// with the deviation from optimality growing with the slack
double      SdMinBall::accuracy (double& slack) const
{
    double e, max_e = 0;
    int n_supp=0;
    Cit i;
    for (i=L.begin(); i!=support_end; ++i,++n_supp)
        if ((e = std::abs (B.excess (*i))) > max_e) max_e = e;

    // you've found a non-numerical problem if the following ever fails
    assert (n_supp == nr_support_points());

    for (i=support_end; i!=L.end(); ++i)
        if ((e = B.excess (*i)) > max_e) max_e = e;

    slack = B.slack();
    return (max_e/squared_radius());
}

// returns true if the accuracy is below the given tolerance and the
// slack is 0
bool        SdMinBall::is_valid (double tolerance) const
{
    double slack;
    return ( (accuracy (slack) < tolerance) && (slack == 0) );
}

