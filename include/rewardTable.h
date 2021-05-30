
struct RewardTable {
  float probability;
  int nextState;
  int reward;
  bool done;
};

typedef struct RewardTable RewardTable;
