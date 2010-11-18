//    Copright (C) 2007, Minh-Tri Pham
//
//    This is a re-implememtation from scratch of Bernd Gaertner's idea at http://www.inf.ethz.ch/personal/gaertner
#ifndef __SD_MINBALL_H
#define __SD_MINBALL_H

#include <list>
#include "sdcpp.h"


// Data Structures
// ============
// point in dimension d
typedef VALARRAY<double> SdVector;
typedef VALARRAY<SdVector> SdVectorArray;


// Class Definitions
// =================

// SdMinBall_core
// ----------
// smallest ball with a set of n <= d+1 points *on the boundary*
class SdMinBall_core {
private:

    // data members
    int                 d;      // dimension
    int                 m, s;   // size and number of support points
    SdVector               q0, z, f;

    SdVectorArray          v, a;

    SdVectorArray          c;
    SdVector               sqr_r;

    SdVector &             current_c;      // refers to some c[j]
    double              current_sqr_r;

    // if you want the copy constructor and assignment operator, please
    // properly implement them yourself. The default copy/assignment
    // semantics fails since a SdMinBall_core object stores pointers to
    // dynamically allocated memory
    SdMinBall_core (const SdMinBall_core& mb);
    SdMinBall_core& operator=(const SdMinBall_core& mb);

public:
    SdMinBall_core(int dim);

    // access
    const SdVector &       center() const;
    double              squared_radius() const;
    int                 size() const;
    int                 support_size() const;
    double              excess (const SdVector& p) const;


    // modification
    void                reset(); // generates empty sphere with m=s=0

    bool                push (const SdVector& p);
    void                pop ();

    // checking
    double              slack() const;
};




// SdMinBall
// ---------
// smallest enclosing ball of a set of n points in dimension d
class SdMinBall {
public:
    // types
    typedef std::list<SdVector>::iterator         It;
    typedef std::list<SdVector>::const_iterator   Cit;

private:
    // data members
    int  d;                 // dimension
    std::list<SdVector> L;  // internal point set
    SdMinBall_core B;            // the current ball
    It support_end;         // past-the-end iterator of support set

    // private methods
    void        mtf_mb (It i);
    void        pivot_mb (It i);
    void        move_to_front (It j);
    double      max_excess (It t, It i, It& pivot) const;

public:
    // creates an empty ball
    SdMinBall(int dim);

    // copies p to the internal point set
    void        check_in (const SdVector& p);

    // builds the smallest enclosing ball of the internal point set
    void        build ();

    // returns center of the ball (undefined if ball is empty)
    const SdVector &      center() const;

    // returns squared_radius of the ball (-1 if ball is empty)
    double      squared_radius () const;

    // returns size of internal point set
    int         nr_points () const;

    // returns begin- and past-the-end iterators for internal point set
    Cit         points_begin () const;
    Cit         points_end () const;

    // returns size of support point set; this set has the following properties:
    // - there are at most d+1 support points,
    // - all support points are on the boundary of the computed ball, and
    // - the smallest enclosing ball of the support point set equals the
    //   smallest enclosing ball of the internal point set
    int         nr_support_points () const;

    // returns begin- and past-the-end iterators for internal point set
    Cit         support_points_begin () const;
    Cit         support_points_end () const;

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
    double      accuracy (double& slack) const;

    // returns true if the accuracy is below the given tolerance and the
    // slack is 0
    bool        is_valid (double tolerance = 1e-15) const;
};


#endif
