fn main() {
    { /* Example 1 */
        print!("Example 1 ");
        let timer = std::time::Instant::now();
        let ans = part1::solution("example-input.txt");
        println!("({:?}):\t{}", timer.elapsed(), ans);
    }

    { /* Part 1 */
        print!("Part 1 ");
        let timer = std::time::Instant::now();
        let ans = part1::solution("input.txt");
        println!("({:?}):\t{}", timer.elapsed(), ans);
    }

    { /* Example 2 */
        print!("Example 2 ");
        let timer = std::time::Instant::now();
        let ans = part2::solution("example-input.txt");
        println!("({:?}):\t{}", timer.elapsed(), ans);
    }

    { /* Part 2 */
        print!("Part 2 ");
        let timer = std::time::Instant::now();
        let ans = part2::solution("input.txt");
        println!("({:?}):\t{}", timer.elapsed(), ans);
    }
}

mod part1 {
    use std::cmp::Ordering;
    use counter::Counter;


    fn get_card_value(card: char) -> u64 {
        if card >= '2' && card <= '9' {
            card.to_digit(10).expect("invalid card number").into()
        } else  {
            match card {
                'T' => 10,
                'J' => 11,
                'Q' => 12,
                'K' => 13,
                'A' => 14,
                _ => 0
            }
        }
    }

    #[derive(PartialEq, Eq, PartialOrd, Ord, Debug)]
    pub enum HandType {
        HighCard = 0,
        OnePair = 1,
        TwoPair = 2,
        ThreeOfAKind = 3,
        FullHouse = 4,
        FourOfAKind = 5,
        FiveOfAKind = 6
    }

    #[derive(PartialEq, Eq, Debug)]
    struct Hand {
        hand: [char; 5],
        strength: HandType,
        bid: u64
    }

    impl Ord for Hand {
        fn cmp(&self, other: &Self) -> Ordering {
            let strength_cmp = self.strength.cmp(&other.strength);
            if strength_cmp == Ordering::Equal {
                self.hand.map(get_card_value).cmp(&other.hand.map(get_card_value))
            } else {
                strength_cmp
            }
        }
    }

    impl PartialOrd for Hand {
        fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
            Some(self.cmp(other))
        }
    }

    fn parse_hand(s: String) -> Hand {
        let hand: [char; 5] = s.split_ascii_whitespace().nth(0).unwrap().chars().collect::<Vec<_>>().try_into().unwrap();

        let card_counts = hand.iter().collect::<Counter<_>>().most_common_ordered();
        let num_unique = card_counts.len();
        let strength = if num_unique == 1 {
            HandType::FiveOfAKind
        } else if num_unique == 2 && card_counts[0].1 == 4 {
            HandType::FourOfAKind
        } else if num_unique == 2 && card_counts[0].1 == 3 {
            HandType::FullHouse
        } else if num_unique == 3 && card_counts[0].1 == 3 {
            HandType::ThreeOfAKind
        } else if num_unique == 3 && card_counts[0].1 == 2 && card_counts[1].1 == 2 {
            HandType::TwoPair
        } else if num_unique == 4 && card_counts[0].1 == 2 {
            HandType::OnePair
        } else {
            HandType::HighCard
        };

        Hand {
            hand: hand,
            strength: strength,
            bid: s.split_ascii_whitespace().nth(1).expect("no bid").parse().expect("error parsing")
        }
    }

    pub fn solution(fpath: &str) -> u64 {
        let mut hands: Vec<_> = utilities::io::read_file_lines(fpath).into_iter().map(parse_hand).collect();
        hands.sort();
        hands.iter().enumerate()
            .map(|(idx, hand)| ((idx as u64)+1) * hand.bid)
            .sum()
    }
}


mod part2 {
    use std::cmp::Ordering;
    use counter::Counter;
    use crate::part1::HandType;

    #[derive(PartialEq, Eq, Debug)]
    struct Hand {
        hand: [char; 5],
        strength: HandType,
        bid: u64
    }

    impl Ord for Hand {
        fn cmp(&self, other: &Self) -> Ordering {
            let strength_cmp = self.strength.cmp(&other.strength);
            if strength_cmp == Ordering::Equal {
                self.hand.map(get_card_value).cmp(&other.hand.map(get_card_value))
            } else {
                strength_cmp
            }
        }
    }

    impl PartialOrd for Hand {
        fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
            Some(self.cmp(other))
        }
    }

    fn get_card_value(card: char) -> u64 {
        if card >= '2' && card <= '9' {
            card.to_digit(10).expect("invalid card number").into()
        } else  {
            match card {
                'T' => 10,
                'J' => 1,   /* special for part 2 */
                'Q' => 12,
                'K' => 13,
                'A' => 14,
                _ => 0
            }
        }
    }

    fn parse_hand(s: String) -> Hand {
        let hand: [char; 5] = s.split_ascii_whitespace().nth(0).unwrap().chars().collect::<Vec<_>>().try_into().unwrap();

        /* most common non j card */
        let most_common = hand.iter().collect::<Counter<_>>().k_most_common_ordered(2)
            .into_iter().find(|(&ch, _)| ch != 'J').or(Some((&'J',5))).expect("error counting Js");
        let new_hand = hand.map(|c| if c == 'J' { most_common.0.clone() } else { c });

        let card_counts = new_hand.iter().collect::<Counter<_>>().most_common_ordered();
        let num_unique = card_counts.len();
        let strength = if num_unique == 1 {
            HandType::FiveOfAKind
        } else if num_unique == 2 && card_counts[0].1 == 4 {
            HandType::FourOfAKind
        } else if num_unique == 2 && card_counts[0].1 == 3 {
            HandType::FullHouse
        } else if num_unique == 3 && card_counts[0].1 == 3 {
            HandType::ThreeOfAKind
        } else if num_unique == 3 && card_counts[0].1 == 2 && card_counts[1].1 == 2 {
            HandType::TwoPair
        } else if num_unique == 4 && card_counts[0].1 == 2 {
            HandType::OnePair
        } else {
            HandType::HighCard
        };

        Hand {
            hand: hand,
            strength: strength,
            bid: s.split_ascii_whitespace().nth(1).expect("no bid").parse().expect("error parsing")
        }
    }

    pub fn solution(fpath: &str) -> u64 {
        let mut hands: Vec<_> = utilities::io::read_file_lines(fpath).into_iter().map(parse_hand).collect();
        hands.sort();
        hands.iter().enumerate()
            .map(|(idx, hand)| ((idx as u64)+1) * hand.bid)
            .sum()
    }
}
