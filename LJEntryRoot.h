/*
 LJKit: an Objective-C implementation of the LiveJournal client protocol
 Copyright (C) 2002-2003  Benjamin Peter Ragheb

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

 You may contact the author via email at benzado@livejournal.com.
 */
/*
 2004-02-24 [BPR] Fixed a typo in the comments.  Changed
                  groupsAllowedAccessArray/Set to return nil instead of raising
                  an exception.
 2004-03-13 [BPR] Added account method.
 
 */

#import <Foundation/Foundation.h>

@class LJAccount, LJJournal, LJGroup;

/*!
@enum LJEntry Security Modes

 @discussion
 These constants define the security modes.

 @constant LJPublicSecurityMode
 This entry is visible by everybody.

 @constant LJPrivateSecurityMode
 This entry is visible only to the author.

 @constant LJFriendSecurityMode
 This entry is visible to anyone on the author's friends list.

 @constant LJGroupSecurityMode
 This entry is visible to anyone in the specified friend groups.
 Specify groups using allowAccessByGroup: and denyAccessByGroup:.
 */
enum {
    LJPublicSecurityMode = 0,
    LJPrivateSecurityMode,
    LJFriendSecurityMode,
    LJGroupSecurityMode
};

/*!
 @const LJEntryWillRemoveFromJournalNotification
 Posted before an entry is removed from the server.  The notification object
 is the instance of LJEntry or LJEntrySummary that is being removed.
 */
FOUNDATION_EXPORT NSString * const LJEntryWillRemoveFromJournalNotification;

/*!
 @const LJEntryDidRemoveFromJournalNotification
 Posted after an entry is successfully removed from the server.  The
 notification object is the instance of LJEntry or LJEntrySummary that was
 removed.
 */
FOUNDATION_EXPORT NSString * const LJEntryDidRemoveFromJournalNotification;

/*!
 @const LJEntryDidNotRemoveFromJournalNotification
 Posted after an attempt to remove an entry from the server fails.  The
 notification object is the instance of LJEntry or LJEntrySummary, and the
 userInfo key @"LJException" is the exception that was raised while the
 attempt was made.
 */
FOUNDATION_EXPORT NSString * const LJEntryDidNotRemoveFromJournalNotification;

/*!
 @class LJEntryRoot
 @discussion
 LJEntryRoot is an abstract class which represents a LiveJournal entry.  It
 primarily exists to share common code between LJEntry and LJEntrySummary.
 LJEntryRoot handles all journal entry meta-data; the subject and the content
 fields are handled by its two subclasses.
 */
@interface LJEntryRoot : NSObject <NSCoding>
{
    LJAccount *_account;
    LJJournal *_journal;
    int _itemID, _aNum;
    NSDate *_date;
    NSString *_posterUsername;
    NSString *_content;
    int _security;
    unsigned int _allowGroupMask;
}

/*!
 @method initWithContentsOfFile:
 @abstract Initializes an entry object from a file.
 @param The file to read from.
 @discussion
 Loads an entry from an archived file.  This method uses NSKeyedUnarchiver
 to read the archive from disk.
 */
- (id)initWithContentsOfFile:(NSString *)path;

/*!
 @method writeToFile:
 @abstract Writes an entry to disk.
 @param path The file to write to.
 @discussion
 Saves an entry to disk.  Uses NSKeyedArchiver to archive the object.
 @result YES on success; NO otherwise.
 */
- (BOOL)writeToFile:(NSString *)path;

/*!
 @method journal
 @abstract Obtain the journal associated with the receiver.
 @discussion
 Returns the journal that the receiver is associated with, or nil if it is
 unassociated.
 */
- (LJJournal *)journal;

/*!
 @method account
 @abstract Obtain the account associated with the receiver.
 @discussion
 Returns the account that the receiver is associated with, or nil if it is
 unassociated.
 */
- (LJAccount *)account;

/*!
 @method posterUsername
 @abstract Obtain the username of the entry's poster.
 @discussion
 Returns the username of the user that posted this entry, according to the
 LiveJournal server.  You should assume that the receiver was posted by the
 currently logged in user if this method returns nil.
 */
- (NSString *)posterUsername;

/*!
 @method itemID
 @abstract Obtain the itemID of the receiver.
 @discussion
 If the entry is posted, returns its itemID as reported by the server.
 If the entry is only associated or unassociated, returns 0.
 */
- (int)itemID;

/*!
 @method date
 @abstract Obtain the date of the receiver.
 */
- (NSDate *)date;

/*!
 @method securityMode
 @abstract Obtain the security mode of the receiver.
 @discussion
 The security modes are explained in the LJEntry Security Modes enumeration.
 */
- (int)securityMode;

/*!
 @method accessAllowedForGroup:
 @abstract Determine whether access is allowed for a specific group.
 @discussion
 Returns YES if the members of the given group are allowed to read the
 receiver, NO otherwise.  This method returns a correct response regardless
 of the receiver's security mode.

 You cannot use this and other group security related methods on unassociated
 entries.  If you try, an exception will be raised.  This is because groups
 have no meaning outside of their account, and unassociated entries are not
 attached to an account.
 */
- (BOOL)accessAllowedForGroup:(LJGroup *)group;

/*!
 @method groupsAllowedAccessMask
 @discussion
 Returns the bitmask which defines the groups allowed to access the receiver.
 If the security mode is not LJGroupSecurityMode, the value is undefined.
 */
- (unsigned int)groupsAllowedAccessMask;

/*!
 @method groupsAllowedAccessArray
 @abstract Determine what groups are allowed access to the receiver.
 @discussion
 If the entry is not associated with a journal, returns nil.  Groups
 have no meaning outside of their account, and unassociated entries are not
 attached to an account.
  If the entry is associated but no groups are allowed access, returns an empty array.
 @result An NSArray of LJGroup objects.
 */
- (NSArray *)groupsAllowedAccessArray;

/*!
 @method groupsAllowedAccessSet
 @abstract Determine what groups are allowed access to the receiver.
 @discussion
 If the entry is not associated with a journal, returns nil.  Groups
 have no meaning outside of their account, and unassociated entries are not
 attached to an account.
 If the entry is associated but no groups are allowed access, returns an empty set.
 @result An NSSet of LJGroup objects.
 */
- (NSSet *)groupsAllowedAccessSet;

/*!
 @method removeFromJournal
 @abstract Removes the receiver from the server.
 @discussion
 If the receiver is a posted entry, sends an editevent message to the server
 so that the entry is deleted.  The receiver becomes associated with the same
 journal but no longer posted.  If this method is called on an associated or
 unassociated entry an exception is raised.
 This method marks the receiver as edited.
 */
- (void)removeFromJournal;

@end
