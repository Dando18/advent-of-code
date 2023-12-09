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

    pub fn get_diffs(v: &Vec<i64>) -> Vec<i64> {
        v.windows(2).map(|x| x[1] - x[0]).collect()
    }

    fn predict_value(vals: &Vec<i64>) -> i64 {
        let mut diffs = vals.clone();
        let mut last_vals: Vec<i64> = Vec::new();

        while !diffs.iter().all(|&d| d == 0) {
            last_vals.push(diffs.last().expect("no last element").clone());
            diffs = get_diffs(&diffs);
        }

        last_vals.iter().sum()
    }

    pub fn parse_values(s: &str) -> Vec<i64> {
        s.split_ascii_whitespace().map(|s| s.parse().expect("parse error")).collect()
    }

    pub fn solution(fpath: &str) -> i64 {
        utilities::io::read_file_lines(fpath).iter()
            .map(|s| parse_values(s))
            .map(|v| predict_value(&v))
            .sum()
    }
}

mod part2 {
    use crate::part1::{parse_values, get_diffs};

    fn predict_value(vals: &Vec<i64>) -> i64 {
        let mut diffs = vals.clone();
        let mut first_vals: Vec<i64> = Vec::new();

        while !diffs.iter().all(|&d| d == 0) {
            first_vals.push(diffs.first().expect("no last element").clone());
            diffs = get_diffs(&diffs);
        }

        first_vals.iter()
            .enumerate()
            .map(|(idx, &val)| if idx % 2 == 0 { val } else { -1 * val })
            .sum()
    }

    pub fn solution(fpath: &str) -> i64 {
        utilities::io::read_file_lines(fpath).iter()
            .map(|s| parse_values(s))
            .map(|v| predict_value(&v))
            .sum()
    }
}
