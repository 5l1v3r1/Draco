//----------------------------------*-C++-*----------------------------------//
/*!
 * \file   SpinLock.hh
 * \author Geoffrey Furnish
 * \date   Fri Dec 16 13:29:01 1994
 * \brief  A spin lock class.  Serializes execution of a blcok.
 * \note   Copyright (C) 1995-2010 Los Alamos National Security, LLC.
 */
//---------------------------------------------------------------------------//
// $Id$
//---------------------------------------------------------------------------//

#ifndef __c4_SpinLock_hh__
#define __c4_SpinLock_hh__

#include "NodeInfo.hh"
#include "Sync.hh"
#include <cstdio>

namespace rtt_c4
{

//===========================================================================//
// class SpinLock - Serialize execution of a block.

// This class enables you to get a block of code to execute serially.  Each
// processor begins to executes the block only after the one before it is
// finished.
//===========================================================================//

class SpinLock : public NodeInfo
{
    SpinLock( const SpinLock& );
    SpinLock& operator=( const SpinLock& );

    enum { SL_Next = 92874 };

    int trash;
    int lock;

  public:
    SpinLock( int _lock =1 );
    ~SpinLock();
};

//===========================================================================//
// class HSyncSpinLock - Serialize a block, syncing at top.

// A spinlock that forces a global sync at the head of the block.
//===========================================================================//

class HSyncSpinLock : public HSync, public SpinLock {

    HSyncSpinLock( const HSyncSpinLock& );
    HSyncSpinLock& operator=( const HSyncSpinLock& );

  public:
    HSyncSpinLock( int l =1 ) : HSync(l), SpinLock(l) {}
};

//===========================================================================//
// class TSyncSpinLock - Serialize a block, syncing at bottom.

// A spinlock that forces a global sync at the tail of the block.
//===========================================================================//

class TSyncSpinLock : public TSync, public SpinLock {

    TSyncSpinLock( const TSyncSpinLock& );
    TSyncSpinLock& operator=( const TSyncSpinLock& );

  public:
    TSyncSpinLock( int l =1 ) : TSync(l), SpinLock(l) {}
};

//===========================================================================//
// class HTSyncSpinLock - Serialize a block, syncing at top and bottom.

// A spinlock that forces a global sync at the head and tail of the block.
//===========================================================================//

class HTSyncSpinLock : public HSync, public TSync, public SpinLock {

    HTSyncSpinLock( const HTSyncSpinLock& );
    HTSyncSpinLock& operator=( const HTSyncSpinLock& );

  public:
    HTSyncSpinLock( int l =1 ) : HSync(l), TSync(l), SpinLock(l) {}
};

#define SPINLOCK(a) \
{ \
    cout << flush; \
    fflush(stdout); \
    C4::HTSyncSpinLock h; \
    a; \
    cout << flush; \
}

} // end of rtt_c4

//---------------------------------------------------------------------------//
// Backwards compatibility

namespace C4
{

using rtt_c4::HSyncSpinLock;
using rtt_c4::TSyncSpinLock;
using rtt_c4::HTSyncSpinLock;

}

#endif                          // __c4_SpinLock_hh__

//---------------------------------------------------------------------------//
//                              end of c4/SpinLock.hh
//---------------------------------------------------------------------------//