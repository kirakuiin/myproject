"""组合转向行为
"""

import case
import math2d
from gameengine import uiobject
from gameengine import defines as color
from . import defines
from . import algorithm


@case.register_case(__name__)
class BlendSteeringCase(case.Case):
    """混合转向"""
    def init_case(self):
        pass

    def update(self, dt):
        pass
