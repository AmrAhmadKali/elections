# elections
A ranked-choice voting system that enable users to provide an ordered list of the candidiates according to their preferences

## Background
In a ranked-choice system (also called tideman system), voters can vote for more than one candidate. Instead of just voting for their top choice, they can rank the candidates in order of preference.
Generally speaking, the Tideman method works by constructing a “graph” of candidates, where an arrow (i.e. edge) from candidate A to candidate B indicates that candidate A wins against candidate B in a head-to-head matchup.
So in the following ballots

![image](https://user-images.githubusercontent.com/69507462/221947825-67c0bee4-8c0a-4259-be00-dc3b5353173f.png)

The according graph would be 

![image](https://user-images.githubusercontent.com/69507462/221948507-71f137fc-9091-4190-8927-55c0ff40bc2b.png)

Once the graph is made, the source of the graph (the one with no edges pointing towards it) is the winner!

