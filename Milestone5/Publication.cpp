/*
Final Project Milestone 5
Filename: Publication.cpp
ID: 105444228
Author:	Cris Huynh
Revision History
-----------------------------------------------------------
Date        Reason
2023/07/02  Preliminary release
2023/?/?    Debugged DMA
-----------------------------------------------------------
I have done all the coding by myself and only copied the code
that my professor provided to complete my workshops and assignments.
-----------------------------------------------------------
MILESTONE JOURNAL:

MS1:
07/03: Beginning the MS1
07/05: Debugging Error
07/08: I'm confused whether create the default construcor or just need to create custom constructor
07/09: Segmentation Fault (Solved by Professor Jeevan Pant)
07/12: Menu Debugging Error, the problem is viewMenu and viewTitle, I separated them out because
it was easy to call the ostream helper function (Helper: Sean Muniz and Sukhman Hara)
07/13: Date Debugging Error, read and write function. cin failed didn't even print out.
I tried many times until I put os << displayTitle(os); (Initial I didnt put os) (Helper: Sean Muniz)
07/13: Small display error, and finished the MS1

MS2:
07/10: Start working on MS2, set attributes and implement LibApp.cpp
07/13: Start debugging on MS2, i didnt know how to implement the confirm when i need to
add Yes. Until my friend help me for using the attribute << "Yes". (Helper: Preet Patel)
07/14: Run function's errors because my loop was infinite until I saw that the issue
that I got is i made a wrong bool value when inside the loop (true ==> false, Helper: Mandhav and Preet Patel)
07/14: Finished

MS3:
07/21: Started MS3
07/22: Debug errors (Segmentation Fault)
07/24: I was stuck with copy assignment and copy constructor(Helper: Sean Muniz, Kabir Nalula)
07/25: Stuck with write and read function (Helper: Preet Patel, Nand Patel, Sukhman Hara)
07/28: Fix some format error and fill char, finished


MS4:
07/25: Started declare and implement the function
07/31: Finished the implementation, but memory leaks and segmentation fault
08/01: Debug the error, the memory leaks on Publication and Book, also
istream error that didnt print 220 lines of book (Helper: Sean Muniz)
08/01: Finised MS4


MS5:
07/19: Started MS5
07/20: Working on method, attributes and functions 
07/23: Implement the cpp file, add PublicSelector modules
07/28: Fixing compiler errors from publication because the format error
07/31: Fixing logic error from the menu, LibApp for the publication selector
08/01: Finished fixing Publication and Menu. Debug error starts with leaks segmentation fault
08/02: Adding Python auto input files, finished the compilation but logic error
08/03: Debug Problem 1: New Publication function, Load function
+ When I tried to input the publication, it didnt ignore the new line and kept me a blank space, 
I was helped my Sean Muniz and Madhav Rajpall for get rid of cin.lcear and cin.ignore() on Publication
beacuse I had already called the ignore on LilApp, and I need to delete one on Publication
+ Load and save function gave me memory leaks, Sean Muniz helped me fixing by passing the dereference instead of
local variable
08/03: Debug Problem 2: Remove and Search function
+ Remove Publication gave me too much ignore line, I decided to get rid of the getline and ignore on the LibApp function
+ Search is the most difficult fuction that I have done for MS5. I couldn't understand what should I return the function(int or void).
Sean Muniz and Sukhman Hara gave me advices and fix the return function so that when I call search for Publication fucntions, I can
call the argument and then it will return the number that it can pass the logic on every Publication function.
08/04: Debug Problem 3: Memory leaks
+ Today, I haven't even test any milestone test beacuse it gave me too much memory leaks.
+ Sean Muniz and Madhav Rajpall helped me to fix the leak by making Destructor, the instruction didnt say anything about it.
Also I just got rid of Default Constructor because the tester calls with argument, so I just needed to create only one 
constructor with a parameter.
08/05: Tested, compilation and endl
+ I just got some formatting error, I forgot endl, forgot to unsetf
+ However the all program was successful
+ Finished MS5 with 6 tests

Milestone form 1 to 5 Helper:
+ Sean Muniz
+ Preet Patel
+ Madhav Rajpall
+ Kabir Narula
+ Nand Patel
+ Joseph Chu
+ Sukhman Hara

-----------------------------------------------------------
*/

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "Publication.h"
#include "Streamable.h"
#include "Date.h"
#include "Lib.h"

using namespace std;
namespace sdds
{
	Publication::Publication() : m_date()
	{
		//todo: Sets all the attributes to their default values
		this->m_title = nullptr;
		this->m_shelfld[0] = '\0';
		this->m_membership = 0;
		this->m_libRef = -1;
	}

	//////////////////////////////////////////////////////////////////////////
	//! Methods
	void Publication::set(int member_id)
	{
		if (member_id > 9999 and member_id < 100000)
		{
			this->m_membership = member_id;
		}
		else
		{
			this->m_membership = 0;
		}
	}

	void Publication::setRef(int value)
	{
		this->m_libRef = value;
	}

	void Publication::resetDate()
	{
		this->m_date = Date();
	}

	char Publication::type() const
	{
		return 'P';
	}

	bool Publication::onLoan() const
	{
		return (this->m_membership != 0);
	}

	Date Publication::checkoutDate() const
	{
		return this->m_date;
	}

	bool Publication::operator==(const char* title) const
	{
		return this->m_title and strstr(this->m_title, title);
	}

	Publication::operator const char* () const
	{
		return this->m_title;
	}

	int Publication::getRef() const
	{
		return this->m_libRef;
	}

	//////////////////////////////////////////////////////////////////////////
	//! Queries
	bool Publication::conIO(ios& io) const
	{
		return (&io == &cin or &io == &cout);
	}

	ostream& Publication::write(ostream& os) const
	{
		// TODO: If the os argument is a console IO object (use conIO()), print shelfId, title, membership 
		// and date attributes as the following format (title is left-justified padded with dots)
		/*
				 1         2         3         4         5         6         7
		1234567890123456789012345678901234567890123456789012345678901234567890
		| P001 | The Toronto Star.............. | 34037 | 2021/11/17 |
		| P007 | Macleans Magazine............. |  N/A  | 2021/11/11 |
		Otherwise:
		Print the type() and then libRef, shelfId, title, membership and date attributes in a
		tab-separated format.
		P	2001		P001		The Toronto Star		34037	2021/11/17
		No newline is printed afterwards either way.
		*/

		if (conIO(os))
		{
			char fc = os.fill(' ');
			os << "| ";
			os.width(SDDS_SHELF_ID_LEN);
			os << this->m_shelfld;

			os << " | ";

			if (strlen(this->m_title) > SDDS_TITLE_WIDTH)
			{
				for (int i = 0; i < SDDS_TITLE_WIDTH; ++i)
				{
					os << m_title[i];
				}
			}
			else 
			{
				os.width(SDDS_TITLE_WIDTH);
				os.setf(ios::left);
				os.fill('.');
				os << this->m_title; // Left-justify the title and pad with dots
				os.fill(fc);
				os.unsetf(ios::left);
			}
			os << " | ";
		
			if (m_membership == 0)
			{
				cout << " N/A ";
			}
			else
			{
				os << this->m_membership;
			}
			
			os << " | ";
			os << this->m_date;
			os << " |";
		}
		else
		{
			os << type();
			os << "\t";
			os << this->m_libRef;
			os << "\t";
			os << this->m_shelfld;
			os << "\t";
			os << this->m_title;
			os << "\t";
			os << this->m_membership;
			os << "\t";
			os << this->m_date;
		}	
		return os;
	}

	istream& Publication::read(istream& is)   //! Helper: Sean Muniz, Sukhman Hara, Preet Patel
	{
		// todo: Read all the values in local variables before setting the attributes to any values.
		/*
		First, clear all the attributes by freeing the memory and setting everything to their default values.

		Then, if the istr argument is a console IO object (use conIO()) read the attributes as follows:

		prompt: "Shelf No: "
		read the shelf number up to its limit (see Lib.h).
		if the number is not exactly the length it is supposed to be, set the istr to a fail state manually.
		prompt: "Title: "
		read the title
		prompt: "Date: "
		read the date

		in this case the libRef value is left with its default value.

		Otherwise, assume reading begins with libRef attribute as if the type 'P' is not in the file.

		read the libRef number
		skip the tab
		read the shelf number
		skip the tab
		read the title
		skip the tab
		read the membership
		skip the tab
		read the date

		Either way if the date is in an invalid state set the istr to a fail state manually

		After the process of reading is done if istr is in a valid state:

		Dynamically store the title into the title attribute
		copy the shelf id into the shelfId attribute
		set the membership
		set the date
		set the libRef attribute

		At the end return the istr argument.	
		*/

		char t_title[256]{}, t_shelfId[SDDS_SHELF_ID_LEN + 1]{};
        int t_libRef = -1, t_membership = 0;
        Date t_date;

        // Clearing the memory if it is allocated and setting the values to default
        m_title = nullptr;
		m_shelfld[0] = '\0';
		m_membership = 0;
		m_libRef = -1;
		resetDate();

        if (conIO(is)) 
		{
            cout << "Shelf No: ";
            is.getline(t_shelfId, SDDS_SHELF_ID_LEN + 1, '\n');

            if (strlen(t_shelfId) != SDDS_SHELF_ID_LEN) 
			{
                is.setstate(ios::failbit);
            }

            std::cout << "Title: ";
            is.getline(t_title, 257, '\n');

            std::cout << "Date: ";
            is >> t_date;
        }
        else 
		{
            is >> t_libRef;
            is.ignore();
            is.getline(t_shelfId, SDDS_SHELF_ID_LEN + 1, '\t');
            is.getline(t_title, 257, '\t');
            is >> t_membership;
            is.ignore();
            is >> t_date;
        }

        if (!t_date) 
		{
            is.setstate(ios::failbit);
        }

        if (is) {
            // Allocate memory and copy the title
            delete[] m_title;
            m_title = nullptr;
            m_title = new char[strlen(t_title) + 1];
            strcpy(m_title, t_title);
            strcpy(m_shelfld, t_shelfId);
            m_membership = t_membership;
            m_date = t_date;
            m_libRef = t_libRef;
        }

        return is;
    }


	Publication::operator bool() const
	{
		//todo: Returns true if neither of the title or shelfId attributes is null or empty. Otherwise, it will return false.
		return (this->m_title != nullptr and this->m_shelfld[0] != '\0') ;
	} 

	//////////////////////////////////////////////////////////////////////////
	//! Rule of Three
	Publication::Publication(const Publication& that) : m_date(that.m_date)
	{
		m_title = nullptr; // Set m_title to nullptr to ensure it doesn't point to any allocated memory
		*this = that; // Use the copy assignment operator to perform a deep copy
	}

	Publication& Publication::operator=(const Publication& that)
	{
		if (this != &that)
		{
			delete[] m_title; // Deallocate existing memory (if any) to avoid memory leaks
			m_title = nullptr; // Set m_title to nullptr to ensure it doesn't point to any allocated memory
			m_shelfld[0] = '\0';

			// Perform a deep copy of the attributes
			m_membership = that.m_membership;
			m_libRef = that.m_libRef;
			m_date = that.m_date;

			// Allocate memory for m_title and copy the contents
			if (that.m_title != nullptr)
			{
				m_title = new char[strlen(that.m_title) + 1];
				strcpy(m_title, that.m_title);
				strcpy(m_shelfld, that.m_shelfld);
			}

			strcpy(m_shelfld, that.m_shelfld);
		}
		return *this;
	}
 
	Publication::~Publication()
	{
		// todo: Make sure this object complies with the guidelines of the Rule of three so the 
		// copying and assignment and destruction are done safely and without any memory leak.
		//todo: Sets all the attributes to their default values.
		 delete[] m_title;
	}
}