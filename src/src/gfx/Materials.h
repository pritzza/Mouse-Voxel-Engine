#pragma once

#include "Lighting.h"

#include "../util/EnumArray.h"

namespace Materials
{
    static constexpr Material VOID{
        { 0.f, 0.f, 0.f },
        { 0.f, 0.f, 0.f },
        { 0.f, 0.f, 0.f },
        0.f
    };

    static constexpr Material EMERALD{
        { 0.0215f,      0.1745f,        0.0215f         },
        { 0.07568f,     0.61424f,       0.07568f        },
        { 0.633f,       0.727811f,      0.633f          },
        76.8f
    };

    static constexpr Material JADE{
        { 0.135f,       0.2225f,        0.1575f         },
        { 0.54f,        0.89f,          0.63f           },
        { 0.316228f,    0.316228f,      0.316228f       },
        12.8f
    };

    static constexpr Material OBSIDIAN{
        { 0.05375f,     0.05f,          0.06625f        },
        { 0.18275f,     0.17f,          0.22525f        },
        { 0.332741f,    0.328634f,      0.346435f       },
        38.4f
    };

    static constexpr Material PEARL{
        { 0.25f,        0.20725f,       0.20725f        },
        { 1.0f,         0.829f,         0.829f          },
        { 0.296648f,    0.296648f,      0.296648f       },
        11.264f
    };

    static constexpr Material RUBY{
        { 0.1745f,      0.01175f,       0.01175f        },
        { 0.61424f,     0.04136f,       0.04136f        },
        { 0.727811f,    0.626959f,      0.626959f       },
        76.8f
    };

    static constexpr Material TURQOISE{
        { 0.1f,         0.18725f,       0.1745f         },
        { 0.396f,       0.74151f,       0.69102f        },
        { 0.297254f,    0.30829f,       0.306678f       },
        12.8f
    };

    static constexpr Material BRASS{
        { 0.329412f,    0.223529f,      0.027451f       },
        { 0.780392f,    0.568627f,      0.113725f       },
        { 0.992157f,    0.941176f,      0.807843f       },
        27.8974f
            };

    static constexpr Material BRONZE{
        { 0.2125f,      0.1275f,        0.054f          },
        { 0.714f,       0.4284f,        0.18144f        },
        { 0.393548f,    0.271906f,      0.166721f       },
        25.6f
    };
    
    static constexpr Material POLISHED_BRONZE{
        { 0.25f,        0.148f,         0.06475f        },
        { 0.4f,         0.2368f,        0.1036f         },
        { 0.774597f,    0.458561f,      0.200621f       },
        76.8f
    };
    
    static constexpr Material CHROME{
        { 0.25f,        0.25f,          0.25f           },
        { 0.4f,         0.4f,           0.4f            },
        { 0.774597f,    0.774597f,      0.774597f       },
        76.8f
    };
    
    static constexpr Material COPPER{
        { 0.19125f,     0.0735f,        0.0225f         },
        { 0.7038f,      0.27048f,       0.0828f         },
        { 0.256777f,    0.137622f,      0.086014f       },
        12.8f
    };
    
    static constexpr Material POLISHED_COPPER{
        { 0.2295f,      0.08825f,       0.0275f         },
        { 0.5508f,      0.2118f,        0.066f          },
        { 0.580594f,    0.223257f,      0.0695701f      },
        51.2f
    };

    static constexpr Material GOLD{
        { 0.24725f,     0.1995f,        0.0745f         },
        { 0.75164f,     0.60648f,       0.22648f        },
        { 0.628281f,    0.555802f,      0.366065f       },
        51.2f
    };

    static constexpr Material POLISHED_GOLD{
        { 0.24725f,     0.2245f,        0.0645f         },
        { 0.34615f,     0.3143f,        0.0903f         },
        { 0.797357f,    0.723991f,      0.208006f       },
        83.2f
    };

    static constexpr Material SILVER{
        { 0.19225f,     0.19225f,       0.19225f        },
        { 0.50754f,     0.50754f,       0.50754f        },
        { 0.508273f,    0.508273f,      0.508273f       },
        51.2f
    };

    static constexpr Material POLISHED_SILVER{
        { 0.23125f,     0.23125f,       0.23125f        },
        { 0.2775f,      0.2775f,        0.2775f         },
        { 0.773911f,    0.773911f,      0.773911f       },
        89.6f
    };

    static constexpr Material TIN{
        { 0.105882f,    0.058824f,      0.113725f       },
        { 0.427451f,    0.470588f,      0.541176f       },
        { 0.333333f,    0.333333f,      0.521569f       },
        9.84615f
    };

    enum class ID
    {
        VOID,
        Emerald,
        Jade,
        Obsidian,
        Pearl,
        Ruby,
        Turquois,
        Brass,
        Bronze,
        PolishedBronze,
        Chrome,
        Copper,
        PolishedCopper,
        Gold,
        PolishedGold,
        Silver,
        PolishedSilver,
        Tin,
        SIZE,
    };

    EnumArray<ID, Material> MATERIALS =
    {
        { ID::VOID,		        Materials::VOID			    },
        { ID::Emerald,		    Materials::EMERALD			},
        { ID::Jade,			    Materials::JADE				},
        { ID::Obsidian,		    Materials::OBSIDIAN			},
        { ID::Pearl,			Materials::PEARL			},
        { ID::Ruby,			    Materials::RUBY				},
        { ID::Turquois,		    Materials::TURQOISE			},
        { ID::Brass,			Materials::BRASS			},
        { ID::Bronze,			Materials::BRONZE			},
        { ID::PolishedBronze,	Materials::POLISHED_BRONZE	},
        { ID::Chrome,			Materials::CHROME			},
        { ID::Copper,			Materials::COPPER			},
        { ID::PolishedCopper,	Materials::POLISHED_COPPER	},
        { ID::Gold,			    Materials::GOLD				},
        { ID::PolishedGold,	    Materials::POLISHED_GOLD	},
        { ID::Silver,			Materials::SILVER			},
        { ID::PolishedSilver,	Materials::POLISHED_SILVER	},
        { ID::Tin,	            Materials::TIN	            },
    };
}
