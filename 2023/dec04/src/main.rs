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
    use std::collections::HashSet;

    pub fn count_matches(line: &String) -> u32 {
        let rhs = line.split(":").nth(1).expect("Invalid line");
        let winners: Vec<u32> = rhs.split("|").nth(0).expect("Invalid line")
            .split_ascii_whitespace().map(|n| n.parse::<u32>().expect("invalid number"))
            .collect();
        let hand: Vec<u32> = rhs.split("|").nth(1).expect("Invalid line")
            .split_ascii_whitespace().map(|n| n.parse::<u32>().expect("invalid number"))
            .collect();

        let winners_unique: HashSet<u32> = HashSet::from_iter(winners);
        let hand_unique: HashSet<u32> = HashSet::from_iter(hand);
        winners_unique.intersection(&hand_unique).into_iter()
            .count().try_into().expect("usize cannot be converted to u32")
    }

    fn score_line(line: &String) -> u32 {
        u32::pow(2, count_matches(line)-1)
    }

    pub fn solution(fpath: &str) -> u32 {
        utilities::io::read_file_lines(fpath).iter().map(score_line).sum()
    }
}

mod part2 {
    use crate::part1;

    pub fn solution(fpath: &str) -> u32 {
        let all_matches: Vec<u32> = utilities::io::read_file_lines(fpath).iter()
            .map(part1::count_matches).collect();

        let mut counts: Vec<u32> = vec![1; all_matches.len()];

        for (idx, n_matches) in all_matches.iter().enumerate() {
            let matches: usize = n_matches.clone() as usize;
            let copies: usize = counts[idx] as usize;

            for _ in 0..copies {    /* for each copy of this card */
                for j in 0..matches {
                    counts[idx + j + 1] += 1;
                }
            }
        }
        counts.iter().sum()
    }
}

