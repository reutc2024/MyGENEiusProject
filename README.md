Genetic Carrier Detection and Risk Simulation Using Deterministic Finite Automaton (DFA)

Description:
This project simulates the genetic inheritance process to identify carrier status for various genetic diseases using a Deterministic Finite Automaton (DFA) implemented in C++. The simulation uses family history data, including genetic information from parents, grandparents, and siblings, to calculate the probability that an individual is a carrier or affected by a specific genetic disorder.

The DFA structure is built using an enum class for states and a TransitionData structure, which stores details such as the next state, transition probability, explanatory message, valid inputs, and a flag for accepting states.

The system supports complex decision logic through a JSON-based transition table, enabling flexible and scalable configuration of inheritance patterns. The probability calculations are based on Mendelian genetics, considering input on known mutations, carrier statuses, and family relations.

Diseases covered include Cystic Fibrosis, Tay-Sachs, Sickle Cell Anemia, Fragile X Syndrome, and Phenylketonuria (PKU). Each disease has its own DNA mutation markers, and part of the system (in Python) parses FASTA files to identify these markers and determine potential carrier status.

The final output includes:

The most probable inherited disease (based on priority and input).

Calculated carrier or affected probabilities.

A full family analysis report based on the automaton transitions.
