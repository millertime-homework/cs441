#!/usr/bin/python

# dinnerparty.py
# CS441 Homework 1
# Copyright Russell Miller 2011
# GPL v3.0 GNU General Public License
# Please see http://www.gnu.org/licenses/gpl.html

import sys

def main():
    """Get the file opened and read"""
    if not len(sys.argv) == 2:
        print("Please provide input filename.")
        sys.exit(1)
    filename = sys.argv[1]
    f = open(filename)
    inputfile = f.read()
    f.close()
    output_filename = filename[:4] + 'soln' + filename[8:]
    print("Creating file %s" % output_filename)
    parse_input(inputfile,output_filename)

def parse_input(inputfile,filename):
    """Find n, Load likeness table"""
    first_newline = inputfile.find('\n')
    if first_newline == -1:
        print("Parse error while finding 'n'")
        sys.exit(1)
    # the first line should only have n on it
    n = int(inputfile[:first_newline])
    # now we can remove the first line
    rest = inputfile[first_newline+1:]
    lines = rest.split('\n')
    likeness = {}
    i = 1
    for line in lines:
        if len(line) < 1:
            continue
        people = line.split(' ')
        likeness[i] = people
        i += 1
    dp = DinnerParty(likeness,n)
    best_score = 0
    best_seating = []
    for i in range(n):
        seats,score = dp.seat_guests(i+1)
        if score > best_score:
            best_score = score
            best_seating = seats
    print_output(best_seating, best_score)
    output_file(best_seating, best_score, filename)

def print_output(seats, points):
    """Just print the output for testing"""
    output = str(points) + '\n'
    people = sorted(seats)
    i = 1
    for person in seats:
        output += str(person) + ' ' + str(i) + '\n'
        i += 1
    print(output)

def output_file(seats, points, filename):
    """Create the output file"""
    output = str(points) + '\n'
    people = sorted(seats)
    i = 1
    for person in seats:
        output += str(person) + ' ' + str(i) + '\n'
        i += 1
    f = open(filename, 'w')
    f.write(output)
    f.close()

def remove_from_list(person,people):
    """Dumb utility function to remove someone from a list"""
    for i in range(len(people)):
        if people[i] == person:
            del people[i]
            break
    return people

class Person:
    """Structure to keep track of each person's id, seat score, and neighbors"""
    def __init__(self,n,score=0,neighbors=None):
        self.n = n

class DinnerParty:
    """Blimped up class that tries different seating arrangements and calculates the scores"""
    def __init__(self,likeness,n,seats_d=None):
        self.likeness = likeness
        self.n = n
        self.seats_d = {}

    def get_unsat_people(self,person):
        """Create a new list 1..n without 'person' in it."""
        people = []
        for i in range(1,self.n+1):
            if i != person:
                people.append(i)
        return people

    def seat_guests(self, starter):
        """Attempting to file them in down-the-rows, 2 at a time.
        Try a different person first each time.
        Then try to swap people one pair at a time to optimize."""
        self.seats_d[1] = Person(starter)
        unsat_people = self.get_unsat_people(self.seats_d[1].n)
        # find most compatible person
        best_friend = self.find_best_friend(self.seats_d[1].n,unsat_people)
        self.seats_d[(self.n/2)+1] = Person(best_friend)
        # and pop them from the list
        unsat_people = remove_from_list(best_friend,unsat_people)
        for i in range(2,(self.n/2)+1):
            # find the best people for the new neighbor seats
            new_top,new_bottom = self.find_best_pair(self.seats_d[i-1].n,self.seats_d[(i-1)+(self.n/2)].n,unsat_people)
            # seat them
            self.seats_d[i] = Person(new_top)
            self.seats_d[i+(self.n/2)] = Person(new_bottom)
            # pop them from the list
            unsat_people = remove_from_list(new_top,unsat_people)
            unsat_people = remove_from_list(new_bottom,unsat_people)
            # calculate how each person is contributing to the score..
        for seat in sorted(self.seats_d.keys()):
            self.seats_d[seat].neighbors = self.get_neighbors(seat)
            self.seats_d[seat].score = self.lookup_seat_score(seat)
        # new algorithm to swap seats for score improvements
        while True:
            swap = self.swap_seats()
            if swap == None:
                break
            a,b = swap
            # print("Swapping seat %d with %d." % (a,b))
            temp = self.seats_d[a]
            self.seats_d[a] = self.seats_d[b]
            self.seats_d[b] = temp
            # re-evaluate their neighbors and score
            self.seats_d[a].neighbors = self.get_neighbors(a)
            self.seats_d[b].neighbors = self.get_neighbors(b)
            self.seats_d[a].score = self.lookup_seat_score(a)
            self.seats_d[b].score = self.lookup_seat_score(b)
        # create a list in order to format the output
        seats = []
        for seat in sorted(self.seats_d.keys()):
            seats.append(self.seats_d[seat].n)
        # seats = [5, 9, 6, 10, 3, 1, 4, 8, 7, 2]    # 100 points! wow
        return (seats, self.calculate_score(seats))

    def find_best_friend(self, person, unsat_people):
        """Find the most compatible person for person, out of whoever hasn't been sat"""
        best_friend = 0
        best_score = -999
        for p in unsat_people:
            like = self.lookup_likeness(person,p)
            if self.mixed_gender(person,p):
                like += 1
                if like > best_score:
                    best_friend = p
                    best_score = like
        return best_friend

    def find_best_pair(self, top, bottom, unsat_people):
        """Seat a new person next to top, and a new one next to bottom
        making sure it's the best combination"""
        best_score = -999
        best_top = 0
        best_bottom = 0
        for x in unsat_people:
            for y in unsat_people:
                if (x != y):
                    like  = self.lookup_likeness(top,x)
                    if self.mixed_gender(top,x):
                        like += 1
                    like += self.lookup_likeness(bottom,y)
                    if self.mixed_gender(bottom,y):
                        like += 1
                    like += self.lookup_likeness(x,y)
                    if self.mixed_gender(x,y):
                        like += 2
                    if like > best_score:
                        best_top = x
                        best_bottom = y
                        best_score = like
        return (best_top,best_bottom)

    def lookup_likeness(self,a,b):
        """See how well two people like each other"""
        return int(self.likeness[a][b-1])+int(self.likeness[b][a-1])

    def mixed_gender(self, person_1, person_2):
        """See if one is in the first half and other in second half"""
        return (person_1 <= (self.n/2) and person_2 > (self.n/2)) or (person_1 > (self.n/2) and person_2 <= (self.n/2))

    def get_neighbors(self, seat):
        """Provide the list of neighbors for a given seat"""
        neighbors = []
        # add person across table
        if seat < ((self.n/2)+1):     # top row
            neighbors.append(self.seats_d[seat+(self.n/2)].n)
        else:                    # bottom row
            neighbors.append(self.seats_d[seat-(self.n/2)].n)
        # if there is a left neighbor
        if seat != ((self.n/2)+1) and seat != 1:
            neighbors.append(self.seats_d[seat-1].n)
        #if there is a right neighbor
        if seat != self.n and seat != (self.n/2):
            neighbors.append(self.seats_d[seat+1].n)
        return neighbors

    def lookup_seat_score(self, seat):
        """See how one person is contributing to the score"""
        score = 0
        person = self.seats_d[seat].n
        # check person across table
        person_across = 0
        if seat < ((self.n/2)+1):     # top row
            person_across = self.seats_d[seat+(self.n/2)].n
        else:                    # bottom row
            person_across = self.seats_d[seat-(self.n/2)].n
        if self.mixed_gender(person_across,person):
            score += 2
        score += self.lookup_likeness(person_across,person)
        # if there is a left neighbor
        if seat != ((self.n/2)+1) and seat != 1:
            person_left = self.seats_d[seat-1].n
            if self.mixed_gender(person_left,person):
                score += 1
            score += self.lookup_likeness(person_left,person)
        #if there is a right neighbor
        if seat != self.n and seat != (self.n/2):
            person_right = self.seats_d[seat+1].n
            if self.mixed_gender(person_right,person):
                score += 1
            score += self.lookup_likeness(person_right,person)
        return score

    def swap_seats(self):
        """Figure out which seats can be swapped to improve the score"""
        swaps = []
        for x in sorted(self.seats_d.keys()):
            for y in sorted(self.seats_d.keys()):
                a = self.seats_d[x]
                b = self.seats_d[y]
                if a.n != b.n and a.n not in b.neighbors and b.n not in a.neighbors:
                    current_combo = a.score + b.score
                    new_combo = 0
                    i = 0
                    for neighbor in b.neighbors:
                        if self.mixed_gender(a.n,neighbor):
                            if i == 0:   # sitting across
                                new_combo += 2
                            else:        # sitting next to
                                new_combo += 1
                        new_combo += self.lookup_likeness(a.n,neighbor)
                        i += 1
                    for neighbor in a.neighbors:
                        if self.mixed_gender(b.n,neighbor):
                            if i == 0:   # sitting across
                                new_combo += 2
                            else:        # sitting next to
                                new_combo += 1
                        new_combo += self.lookup_likeness(b.n,neighbor)
                        i += 1
                    if new_combo > current_combo:
                        # print("Improvement possible! Switch person %d with person %d for a difference of %d" % 
                        #                                   (a.n,b.n,new_combo-current_combo))
                        return (x,y)
        return None

    def calculate_score(self, seats):
        """Using the given rules, calculate this arrangement's score"""
        points = 0
        toprow = seats[:self.n/2]
        bottomrow = seats[self.n/2:]
        for i in range((self.n/2)-1):
            # top row people
            if self.mixed_gender(toprow[i],toprow[i+1]):
                points += 1
            # self.likeness score with person on right
            top_row_right = self.lookup_likeness(toprow[i], toprow[i+1])
            points += top_row_right
            
            # bottom row
            if self.mixed_gender(bottomrow[i],bottomrow[i+1]):
                points += 1
            # self.likeness score with person on right
            bottom_row_right = self.lookup_likeness(bottomrow[i], bottomrow[i+1])
            points += bottom_row_right
            
            # relations across the table
            if self.mixed_gender(toprow[i],bottomrow[i]):
                points += 2
            across_table = self.lookup_likeness(toprow[i], bottomrow[i])
            points += across_table

        # handle the last pair of people across from each other
        if self.mixed_gender(toprow[self.n/2-1], bottomrow[self.n/2-1]):
            points += 2
        last_pair = self.lookup_likeness(toprow[self.n/2-1], bottomrow[self.n/2-1])
        points += last_pair
        return points

if __name__=="__main__":
    main()
