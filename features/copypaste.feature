Feature: Copy and Paste

   Background:
 
   Scenario: Valid JSON can be pasted and copied back
      When she puts the file contents "test/dict.json" on the clipboard
      And she sends key "p" to "/"
      And she invalidates the clipboard
      And she sends key "c" to "/"
      And the clipboard contains the file contents "test/dict.json"

   Scenario: A dictionary can be pasted into an empty dictionary
      When she puts the file contents "test/listofemptydicts.json" on the clipboard
      And she sends key "p" to "/"
      Then "/1" contains 0 elements
      And she puts the file contents "test/dict.json" on the clipboard
      And she sends key "p" to "/1"
      Then "/1" contains 3 elements
      And she sends key "u" to "/"
      And "/1" contains 0 elements
      And she sends key "r" to "/"
      And "/1" contains 3 elements
      And she sends key "u" to "/"
      And "/1" contains 0 elements

   Scenario: A dictionary can be pasted into an empty dictionary in a dict
      When she puts the file contents "test/dictofemptydicts.json" on the clipboard
      And she sends key "p" to "/"
      Then "/1" contains 0 elements
      And she puts the file contents "test/dict.json" on the clipboard
      And she sends key "p" to "/1"
      Then "/1" contains 3 elements
      And she sends key "u" to "/"
      And "/1" contains 0 elements
      And she sends key "r" to "/"
      And "/1" contains 3 elements
      And she sends key "u" to "/"
      And "/1" contains 0 elements
