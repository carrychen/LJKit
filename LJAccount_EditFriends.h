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

#import <Foundation/Foundation.h>
#import "LJAccount.h"

@class LJFriend, LJGroup;

/*!
 @category LJAccount(EditFriends)
 @abstract Methods for editing friends and groups.
 */
@interface LJAccount (EditFriends)

/*!
 @method downloadFriends
 @abstract Download friends and groups information from the server.
 @discussion
 Downloads the friends and groups associated with the receiver.  This method
 causes any changes made to local LJFriend and LJGroup objects to be discarded.
 */
- (void)downloadFriends;

/*!
 @method uploadFriends
 @abstract Upload changes to friends and groups to the server.
 @result YES if changes were made; NO if there were no changes to upload.
 @discussion
 Uploads the local changes to LJFriend objects to the LiveJournal server.
 If friends were added, their full names are obtained from the server
 and saved.  Other information, such as birthdays, won't be updated until
 the next call to downloadFriends.
 */
- (BOOL)uploadFriends;

/*!
 @method friendSet
 @abstract Obtain the friends associated with the receiver.
 @discussion
 Returns an NSSet containing the user's friends in the form of LJFriend objects.
 If the friends list hasn't been downloaded, returns nil.
 This property is preserved during archiving.
*/
- (NSSet *)friendSet;

/*!
 @method friendArray
 @abstract Obtain the friends associated with the receiver.
 @discussion
 Returns a sorted NSArray containing the user's friends in the form of LJFriend
 objects.
 If the friends list hasn't been downloaded, returns nil.
 This property is preserved during archiving.
*/
- (NSArray *)friendArray;

/*!
 @method friendEnumerator
 @abstract Enumerate the friends associated with the receiver.
 @discussion
 Returns an NSEnumerator which traverses the user's friends, returning LJFriend
 objects.
 */
- (NSEnumerator *)friendEnumerator;

/*!
 @method groupSet
 @abstract Obtain the friend groups associated with the receiver.
 @discussion
 Returns an NSSet containing the user's friend groups in the form of LJGroup
 objects.

 This property is preserved during archiving.
*/
- (NSSet *)groupSet;

/*!
 @method groupArray
 @abstract Obtain the friend groups associated with the receiver.
 @discussion
 Returns a sorted NSArray containing the user's friend groups in the form of
 LJGroup objects.

 This property is preserved during archiving.
*/
- (NSArray *)groupArray;

/*!
 @method groupEnumerator
 @abstract Enumerate the groups associated with the receiver.
 @discussion
 Returns an NSEnumerator which traverses the user's groups, returning
 LJGroup objects.
 */
- (NSEnumerator *)groupEnumerator;

/*!
 @method friendOfSet
 @abstract Returns the friends which list the receiver as a friend.
 @discussion
 If the friends list hasn't been downloaded, returns nil.
 This property is preserved during archiving.
 @result An NSSet of LJFriend objects.
 */
- (NSSet *)friendOfSet;

/*!
 @method friendOfArray
 @abstract Returns the friends which list the receiver as a friend.
 @discussion
 If the friends list hasn't been downloaded, returns nil.
 This property is preserved during archiving.
 @result A sorted NSArray of LJFriend objects.
 */
- (NSArray *)friendOfArray;

/*!
 @method friendOfEnumerator
 @abstract Enumerate the "friend of" users for the receiver.
 @discussion
 Returns an NSEnumerator which traverses users who list the receiver as a
 friend, returning LJFriend objects.
 */
- (NSEnumerator *)friendOfEnumerator;

/*!
 @method relationshipArray
 @abstract Returns all the friends with which the receiver has a relationship
 @discussion
 In contrast to -friendArray and -friendOfArray, this method returns
 a sorted array in which are mixed mutual, incoming and outgoing relationships.
 */
- (NSArray *)relationshipArray;

/*!
 @method watchedCommunityArray
 @abstract Returns the communities the receiver is watching.
 If the friends list hasn't been downloaded, returns nil.
 @result A sorted NSArray of LJFriend objects.
 */
- (NSArray *)watchedCommunityArray;

/*!
 @method watchedCommunitySet
 @abstract Returns the communities the receiver is watching.
 If the friends list hasn't been downloaded, returns nil.
 @result An NSSet of LJFriend objects.
*/
- (NSSet *)watchedCommunitySet;

/*!
 @method joinedCommunityArray
 @abstract Returns the communities the receiver is a member of.
 If the friends list hasn't been downloaded, returns nil.
 @result A sorted NSArray of LJFriend objects.
 */
- (NSArray *)joinedCommunityArray;

/*!
 @method joinedCommunitySet
 @abstract Returns the communities the receiver is a member of.
 If the friends list hasn't been downloaded, returns nil.
 @result An NSSet of LJFriend objects.
 */
- (NSSet *)joinedCommunitySet;

/*!
 @method friendNamed:
 @abstract Returns the friend with the given name.
 @discussion
 This method searches both the friends and friend-ofs for the account for the
 friend with the given username.
 */
- (LJFriend *)friendNamed:(NSString *)username;

/*!
 @method addFriend:
 @abstract Add a friend to the receiver's friends list.
 */ 
- (LJFriend *)addFriendWithUsername:(NSString *)username;

/*!
 @method removeFriend:
 @abstract Removes a friend from the receiver's friends list.
 */
- (void)removeFriend:(LJFriend *)buddy;

/*!
 @method newGroupWithName:
 @abstract Create a new group.
 @discussion
 Use this method if you want to add a new friend group.  LiveJournal limits you to
 a total of 30 groups; if you exceed this limit an exception will be raised.
 */
- (LJGroup *)newGroupWithName:(NSString *)name;

/*!
 @method removeGroup:
 @abstract Removes a friend group.
 @discussion
 Removes a friend group from the receiver.  This method also removes all friends
 from the group.  If you decide to cancel the operation by calling downloadGroups,
 remember to call downloadFriends also.

 When you use uploadGroups to commit this change, the server updates the
 groupmask of previous posts and removes references to the deleted groups.
 The LJKit does not attempt to simulate this behavior, as it does not know
 where relevant LJEntry objects are stored.
 */
- (void)removeGroup:(LJGroup *)group;

- (void)updateGroupSetWithReply:(NSDictionary *)reply;

/*!
 @method groupMaskFromSet:
 @discussion
 Returns the bitmask which defines the given set of groups.
 */
- (unsigned int)groupMaskFromSet:(NSSet *)groupSet;

/*!
 @method groupMaskFromArray:
 @discussion
 Returns the bitmask which defines the given array of groups.
 */
- (unsigned int)groupMaskFromArray:(NSArray *)groupArray;

/*!
 @method groupArrayFromMask:
 @discussion
 Returns an array of the groups defined by the given bitmask.
 */
- (NSArray *)groupArrayFromMask:(unsigned int)groupMask;

/*!
 @method groupSetFromMask:
 @discussion
 Returns a set of the groups defined by the given mask.
 */
- (NSSet *)groupSetFromMask:(unsigned int)groupMask;

@end
