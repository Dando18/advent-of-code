
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

    pub struct Game {
        pub id: u32,
        pub red: u32,
        pub blue: u32,
        pub green: u32
    }

    pub fn parse_game(s: &str) -> Game {
        let id: u32 = s.split(":").nth(0).expect("invalid line")
            .split_ascii_whitespace().last().expect("no game id")
            .parse().expect("couldn't parse game id");

        let mut counts = Game { id: id, red: 0, blue: 0, green: 0 };
        s.split(":").nth(1).unwrap()
            .split(";")
            .for_each(|grab| {
                grab.split(",").for_each(|col_and_amt| {
                    let col = col_and_amt.split_ascii_whitespace().nth(1).expect("parsing error");
                    let amt: u32 = col_and_amt.split_ascii_whitespace().nth(0).unwrap().parse().expect("parsing error");
                    match col {
                        "red" => { counts.red = std::cmp::max(counts.red, amt); },
                        "blue" => { counts.blue = std::cmp::max(counts.blue, amt); },
                        "green" => { counts.green = std::cmp::max(counts.green, amt); },
                        _ => ()
                    }
                })
            });
        counts
    }

    fn is_possible(game: &Game) -> bool {
        return game.red <= 12 && game.blue <= 14 && game.green <= 13;
    }

    pub fn solution(fpath: &str) -> u32 {
        utilities::io::read_file_lines(fpath).iter()
            .map(|s| parse_game(s))
            .filter(is_possible)
            .map(|g| g.id)
            .sum()
    }

}

mod part2 {
    use crate::part1;

    pub fn solution(fpath: &str) -> u32 {
        utilities::io::read_file_lines(fpath).iter()
            .map(|s| part1::parse_game(s))
            .map(|g| g.red * g.blue * g.green)
            .sum()
    }
}