# rough code without edge test

from collections import defaultdict

# Read input
n, k = map(int, input().split())

# Dictionary to store the frequency of sorted word forms
# defaultdict(int) creates a dictionary where each value is automatically initialized to 0
anagramCount = defaultdict(int)

# Read each word and count the sorted form
for _ in range(n):
    word = input()
    sortedWord = ''.join(sorted(word))  # Sort the word to a single string and find its anagram form
    anagramCount[sortedWord] += 1   # Increment the count for sorted anagram word

# Initialize the unique count to 0
notAnagramCount = 0

# Iterate through all the counts in the anagram_count dictionary
for count in anagramCount.values():
    # Check if the count of this sorted word form is exactly 1
    if count == 1:
        # If the count is 1, it means this word form is unique (no anagrams)
        notAnagramCount += 1  # Increment the unique count by 1

# Print the result
print(notAnagramCount)
