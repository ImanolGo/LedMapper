/*
 *  Manager.h
 *
 *  Created by Imanol Gomez on 27/02/15.
 *
 */

#pragma once

//========================== class Manager ==============================
//============================================================================
/** \class Manager Manager.h
*	\brief Base class for all managers.
*	\details Provides basic member attributes which are in common for all managers.
*/
//==============================================================================

class Manager
{
	public:

		//! Constructor
		Manager(): m_initialized(false) {}

		//! Destructor
		virtual ~Manager() {}

		//! Set-up the manager
		virtual void setup() {m_initialized=true;}

		//! updates the manager
		virtual void update() {}

		//==========================================================================

    protected:

        bool	m_initialized;	///< defines if the manager has been already been initialized


};

