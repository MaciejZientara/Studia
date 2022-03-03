import random

# 1. royal flush
# 2. straight flush
# 3. four of a kind
# 4. full house
# 5. flush
# 6. straight
# 7. three of a kind
# 8. two pair
# 9. one pair
# 10. high card

# possible sets:
# four of a kind, full house, three of a kind, two pair, one pair
# 3, 4, 7, 8, 9

# black spades (♠), red hearts (♥), blue diamonds (♦) and green clubs (♣)
colors = ['♠','♥','♦','♣']
figures = ['A','K','Q','J']

def drawHandFigures():
    res = []
    while len(res)!=5:
        tmp = random.choice(figures) + random.choice(colors)
        if tmp not in res:
            res.append(tmp)
    return sorted(res)

def whatSet(hand):
    figuresCount = [0,0,0,0]
    for card in hand:
        figuresCount[figures.index(card[0])]+=1
    if 4 in figuresCount:
        print('four of a kind')
        return 3
    if 3 in figuresCount and 2 in figuresCount:
        print('full house')
        return 4
    if 3 in figuresCount:
        print('three of a kind')
        return 7
    pairCount = sum(x==2 for x in figuresCount)
    if pairCount == 2:
        print('two pair')
        return 8
    if pairCount == 1:
        print('one pair')
        return 9
    

hand = drawHandFigures()
print(hand)
print(whatSet(hand))
