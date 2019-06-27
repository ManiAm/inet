# change directory to Desktop
cd ~/Desktop

# clone the forked INET repository
git clone --recurse-submodules https://github.com/ManiAm/inet

# change directory to inet
cd inet

# switch to integration branch
git checkout integration

# switch to the integration branch in showcase 
cd showcases
git checkout integration

# and similarly for tutorial
cd ../tutorials
git checkout integration
