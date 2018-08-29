FEATURE mahjong.h/Impl
typedef std::vector<int> MatchIds;

// Generate ids for 144 tiles according to Mahjong Solitaire rules:
// http://www.rubl.com/rules/mahjong-solitaire-rules.html
MatchIds matchIds()
{
    MatchIds ids;

    // There are 42 different match ids overall.
    const int IDS_COUNT = 42;
    // 35 of them are represented by 4 duplicates. The rest are single.
    const int GROUPED_IDS_COUNT = 35;

    // Generate 4 duplicates for each grouped id.
    // Generate single id for the rest of ids.
    for (int id = 0; id < IDS_COUNT; ++id)
    {
        bool isIdGrouped = (id - GROUPED_IDS_COUNT) < 1;
        int duplicatesCount = isIdGrouped ? 4 : 1;
        for (int duplicateId = 0; duplicateId < duplicatesCount; ++duplicateId)
        {
            ids.push_back(id);
        }
    }
    
    return ids;
}

// Generate ids for any number of tiles by reusing the same 144 ids.
MatchIds matchIds(int count)
{
    MatchIds limitedIds = matchIds();
    MatchIds ids;

    int id = 0;
    for (int i = 0; i < count; ++i)
    {
        int matchId = limitedIds[id];
        ids.push_back(matchId);

        // Reuse.
        if (++id >= limitedIds.size())
        {
            id = 0;
        }
    }

    return ids;
}
