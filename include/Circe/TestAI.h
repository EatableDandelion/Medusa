#pragma once

//Principle:
//Mobs have various goals to improve profit, such as mining directly, hunting or joining a group to share
//work and profit: it is a multiple bandit problem. Each of these goals have weights that determine
//how the mob will act. The weights (=motivation) are based on randomness derivation from the parent 
//initially, but evolving as they learn. For example, if the weight of mining alone is greater than joining a group, the mob 
//will mine alone the weight increases it generate profit, and decrease if it's under attack or starving.
//Eventually other goals will be added, such as "lead group" etc... The weights should evolve in the
//right direction if the unit is good at doing what it does and should constitute a genetic selection of the
//population.

