#pragma once

namespace Cards
{
    enum class Play
    {
        OIL_PLANT,
        BUSSINESS,
        WIND_FARM,
        SOLAR_FARM,
        NUCLEAR_PLANT,
    };

    enum class Event
    {
        SUNNY,
        WINDY,
        BUSINESS_BOOM,
        PANDEMIC,
        OIL_EXPLOSION,
        NUCLEAR_METLDOWN
    };
}