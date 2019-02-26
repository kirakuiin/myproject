--[[ This is dice script for campfire war card game. --]]
-- author:wang.zhuowei@foxmail.com
-- date: Feb 11, 2019
-- license: GPL.v3

--[[ Called whenever a Player picks up an Object. --]]
function onObjectPickUp(color, obj)
    if obj == self then
        self.randomize()
        self.setVelocity({0, 0, 0})
    end
end
