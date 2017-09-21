# Deterministic-Finite-State-Machine

DFA written in C++ that allows user to input any DFAs and input to check for membership and equivalence of two DFAs.

### Softwares

* [Microsoft Visual Studio Community 2017](https://www.visualstudio.com/downloads/)
* [C++ IDE for Visual Studio](https://www.visualstudio.com/vs/cplusplus/)

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.


## Example Input

The format for entering an input text file for the DFA would be:
```
# This DFA recognizes { x in {0,1}* | x does not start in aa }
# qe empty
# q0 start with a
# q00 start with aa
# q000 start with aaa
states: qe; q0; q00; q000
input_alphabet: a; b
start_state: qe # empty when we start
accept_states: qe; q00; q0 # accept as long as the it doesnt start with aaa
# if we see a b, accept: qe,b -> qe; q0,b -> qe; q00,b -> qe
# if we see a a, count one more 0 than before: qe,a -> q0; q0,a -> q00; q00,a -> q000
#everything on one line
delta: qe,b -> qe; q0,b -> q0; q00,b -> q00; q000,b -> q000; qe,a -> q0; q0,a -> q00;q00,a -> q000; q000,a -> q000
```
```
states = name of states 
input_alphanet = input for the DFA 
start_state = inital state
accept_state = end state
delta = transition of DFA and each transition has to be separated with a semi colon
```
Please change the filename in .cpp to your desire input DFA and tinput to your desire input to check for membership

### Documentation 

Explain what these tests test and why

```
Give an example
```







