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

    pub struct Race {
        pub time_limit: u64,
        pub best_distance: u64
    }

    fn parse_numbers_after(s: &str, delim: &str) -> Vec<u64> {
        s.split(delim).nth(1).expect("can't split after delim")
            .split_ascii_whitespace().map(|s| s.parse().expect("cannot parse numbers")).collect()
    }

    fn win_counts(race: Race) -> u64 {
        (0..race.time_limit).into_iter()
            .filter(|bt| (bt * (race.time_limit - bt)) > race.best_distance)
            .count().try_into().expect("Invalid win count")
    }

    pub fn solution(fpath: &str) -> u64 {
        let lines = utilities::io::read_file_lines(fpath);
        let times = parse_numbers_after(&lines[0], ":");
        let distances = parse_numbers_after(&lines[1], ":");

        let races = times.iter().zip(distances.iter())
            .map(|(t,d)| Race {time_limit: t.clone(), best_distance: d.clone()});

        races.map(win_counts).product()
    }
}


mod part2 {
    fn parse_number_after(s: &str, delim: &str) -> u64 {
        s.split(delim).nth(1).expect("can't split after delim").trim()
            .replace(" ", "").parse().expect("could not parse number")
    }

    pub fn solution(fpath: &str) -> u64 {
        let lines = utilities::io::read_file_lines(fpath);
        let time = parse_number_after(&lines[0], ":");
        let distance = parse_number_after(&lines[1], ":");

        (0..time).into_iter()
            .filter(|bt| (bt * (time - bt)) > distance)
            .count().try_into().expect("could not count")
    }
}