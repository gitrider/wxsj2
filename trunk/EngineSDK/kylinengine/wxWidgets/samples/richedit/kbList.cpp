/*-*- c++ -*-********************************************************
 * kbList.cc : a double linked list                                 *
 *                                                                  *
 * (C) 1998 by Karsten Ball�der (Ballueder@usa.net)                 *
 *                                                                  *
 * $Id: kbList.cpp,v 1.4 2005/05/31 09:19:38 JS Exp $          *
 *                                                                  *
 * $Log: kbList.cpp,v $
 * Revision 1.4  2005/05/31 09:19:38  JS
 * Typo correction patch [ 1208110 ] Lots of typo corrections
 * Olly Betts
 *
 * Revision 1.3  2004/08/06 17:27:18  ABX
 * Deleting void is undefined.
 *
 * Revision 1.2  2002/01/16 13:39:50  GT
 * Added checks for wxUSE_IOSTREAMH to determine which iostream(.h) to use
 *
 * Revision 1.1  1999/06/07 09:57:12  KB
 * Formerly known as wxLayout.
 *
 * Revision 1.3  1998/11/19 20:34:50  KB
 * fixes
 *
 * Revision 1.8  1998/09/23 08:57:27  KB
 * changed deletion behaviour
 *
 * Revision 1.7  1998/08/16 21:21:29  VZ
 *
 * 1) fixed config file bug: it was never created (attempt to create ~/.M/config
 *    always failed, must mkdir("~/.M") first)
 * 2) "redesign" of "Folder properties" dialog and bug corrected, small change to
 *    MInputBox (it was too wide)
 * 3) bug in ProvFC when it didn't recognize the books as being in the correct
 *    format (=> messages "can't reopen book") corrected
 * 4) I tried to enhance MDialog_About(), but it didn't really work... oh well,
 *    I've never said I was an artist
 *
 * Revision 1.6  1998/07/08 11:56:56  KB
 * M compiles and runs on Solaris 2.5/gcc 2.8/c-client gso
 *
 * Revision 1.5  1998/06/27 20:07:18  KB
 * several bug fixes for kbList
 * started adding my layout stuff
 *
 * Revision 1.1.1.1  1998/06/13 21:51:12  karsten
 * initial code
 *
 * Revision 1.4  1998/05/24 14:48:00  KB
 * lots of progress on Python, but cannot call functions yet
 * kbList fixes again?
 *
 * Revision 1.3  1998/05/18 17:48:34  KB
 * more list<>->kbList changes, fixes for wxXt, improved makefiles
 *
 * Revision 1.2  1998/05/14 16:39:31  VZ
 *
 * fixed SIGSEGV in ~kbList if the list is empty
 *
 * Revision 1.1  1998/05/13 19:02:11  KB
 * added kbList, adapted MimeTypes for it, more python, new icons
 *
 *******************************************************************/

#ifdef __GNUG__
#   pragma implementation "kbList.h"
#endif

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#  pragma hdrstop
#endif

#include   "wx/wx.h"

#include   "kbList.h"


kbListNode::kbListNode( void *ielement,
                        kbListNode *iprev,
                        kbListNode *inext)
{
   next = inext;
   prev = iprev;
   if(prev)
      prev->next = this;
   if(next)
      next->prev = this;
   element = ielement;
}

kbListNode::~kbListNode()
{
   if(prev)
      prev->next = next;
   if(next)
      next->prev = prev;
}


kbList::iterator::iterator(kbListNode *n)
{
   node = n;
}

void *
kbList::iterator::operator*() 
{
   return node->element;
}

kbList::iterator &
kbList::iterator::operator++()
{
   node  = node ? node->next : NULL;
   return *this;
}

kbList::iterator &
kbList::iterator::operator--()
{
   node = node ? node->prev : NULL; 
   return *this;
}
kbList::iterator &
kbList::iterator::operator++(int /* foo */)
{
   return operator++();
}

kbList::iterator &
kbList::iterator::operator--(int /* bar */)
{
   return operator--();
}


bool
kbList::iterator::operator !=(kbList::iterator const & i) const
{
   return node != i.node;
}

bool
kbList::iterator::operator ==(kbList::iterator const & i) const
{
   return node == i.node;
}

kbList::kbList(bool ownsEntriesFlag)
{
   first = NULL;
   last = NULL;
   ownsEntries = ownsEntriesFlag;
}

void
kbList::push_back(void *element)
{
   if(! first) // special case of empty list
   {
      first = new kbListNode(element);
      last = first;
      return;
   }
   else
      last = new kbListNode(element, last);
}

void
kbList::push_front(void *element)
{
   if(! first) // special case of empty list
   {
      push_back(element);
      return;
   }
   else
      first = new kbListNode(element, NULL, first);
}

void *
kbList::pop_back(void)
{
   iterator i;
   void *data;
   bool ownsFlagBak = ownsEntries;
   i = tail();
   data = *i;
   ownsEntries = false;
   erase(i);
   ownsEntries = ownsFlagBak;
   return data;
}

void *
kbList::pop_front(void)
{
   iterator i;
   void *data;
   bool ownsFlagBak = ownsEntries;
   
   i = begin();
   data = *i;
   ownsEntries = false;
   erase(i);
   ownsEntries = ownsFlagBak;
   return data;
   
}

void
kbList::insert(kbList::iterator & i, void *element)
{   
   if(! i.Node())
      return;
   else if(i.Node() == first)
   {
      push_front(element);
      i = first;
      return;
   }
   i = kbList::iterator(new kbListNode(element, i.Node()->prev, i.Node()));
}

void
kbList::doErase(kbList::iterator & i)
{
   kbListNode
      *node = i.Node(),
      *prev, *next;
   
   if(! node) // illegal iterator
      return;

   prev = node->prev;
   next = node->next;
   
   // correct first/last:
   if(node == first)
      first = node->next;
   if(node == last)  // don't put else here!
      last = node->prev;

   // build new links:
   if(prev)
      prev->next = next;
   if(next)
      next->prev = prev;

   // delete this node and contents:
   // now done separately
   //if(ownsEntries)
   //delete *i;
   delete i.Node();

   // change the iterator to next element:
   i = kbList::iterator(next);
}

kbList::~kbList()
{
   kbListNode *next;

   while ( first != NULL )
   {
      next = first->next;
      if(ownsEntries)
      {
#if 0
         delete first->element;
#else
         wxLogError(wxT("Deleting `void*' is undefined."));
         wxLogError(wxT("Entries of kbList should be deleted by destructors of derived classes."));
#endif
      }
      delete first;
      first = next;
   }
}

kbList::iterator
kbList::begin(void) const
{
   return kbList::iterator(first);
}

kbList::iterator
kbList::tail(void) const
{
   return kbList::iterator(last);
}

kbList::iterator
kbList::end(void) const
{
   return kbList::iterator(NULL); // the one after the last
}

unsigned
kbList::size(void) const // inefficient
{
   unsigned count = 0;
   kbList::iterator i;
   for(i = begin(); i != end(); i++, count++)
      ;
   return count;
}







#ifdef   KBLIST_TEST

#if wxUSE_IOSTREAMH
    #include <iostream.h>
#else
    #include <iostream>
#endif

KBLIST_DEFINE(kbListInt,int);
   
int main(void)
{
   int
      n, *ptr;
   kbListInt
      l;
   kbListInt::iterator
      i;
   
   for(n = 0; n < 10; n++)
   {
      ptr = new int;
      *ptr = n*n;
      l.push_back(ptr);
   }

   i = l.begin(); // first element
   i++; // 2nd
   i++; // 3rd
   i++; // 4th, insert here:
   ptr = new int;
   *ptr = 4444;
   l.insert(i,ptr);

   // this cannot work, because l.end() returns NULL:
   i = l.end(); // behind last
   i--;  // still behind last
   l.erase(i);  // doesn't do anything

   // this works:
   i = l.tail(); // last element
   i--;
   --i;
   l.erase(i); // erase 3rd last element (49)
   
   for(i = l.begin(); i != l.end(); i++)
      cout << *i << '\t' << *((int *)*i) << endl;

   
   return 0;
}
#endif
