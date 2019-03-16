A project to set up an ecosystem simulation

Create each species as an object:
Unique species ID
Species name
Pregnancy duration
Breeding rate (mating cooldown)
Sex that becomes pregnant
What they can eat (Herbivore, Carnivore, Omnivore, Carrion)
How much they need to eat
Lifespan
Age of maturity
Amount of food they provide when eaten (Baby, Young, Adult, Old)
Time for matter to rot (in a given environment, that environment should have rot modifier of 1)

Individual creature
Species ID
Are they living (if false change to dead creature, meat will rot)
Age
Health
Hunger
Sex
How long they've been pregnant for (0 for not pregnant)
Time since last mated

Dead Creature
Species ID
Length of time dead
Amount of fresh meat
Amount of carrion

Environment
Season
Spring max/ min temp
Summer max/ min temp
Autumn max/ min temp
Winter max/ min temp
Spring max/ min precipitaion
Summer max/ min precipitaion
Autumn max/ min precipitaion
Winter max/ min precipitaion
Current temperature (dependant on season)
Current precipitaion (dependant on season)
Rot modifier (dependant on temp and precipitation)