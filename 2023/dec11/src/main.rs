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
    pub trait Distance<T> {
        fn distance(&self, other: &Self) -> T;
    }

    #[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
    pub struct Coord<T> {
        row: T,
        col: T,
    }

    impl Distance<u64> for Coord<u64> {
        fn distance(&self, other: &Self) -> u64 {
            let row_dist = if self.row > other.row {
                self.row - other.row
            } else {
                other.row - self.row
            };
            let col_dist = if self.col > other.col {
                self.col - other.col
            } else {
                other.col - self.col
            };
            (row_dist + col_dist) as u64
        }
    }

    type Grid = Vec<Vec<char>>;

    fn is_column_all(grid: &Grid, c: usize, ch: char) -> bool {
        grid.iter().all(|row| row[c] == ch)
    }

    fn is_row_all(grid: &Grid, r: usize, ch: char) -> bool {
        grid[r].iter().all(|&c| c == ch)
    }

    pub fn get_galaxies(grid: &Grid, galaxy_ch: char, space_char: char, spacing: u64) -> Vec<Coord<u64>> {
        let space_cols = (0..grid[0].len())
            .map(|c| if is_column_all(grid, c, space_char) { spacing } else { 1 } )
            .scan(0, |acc, x| {
                *acc += x;
                Some(*acc)
            }).collect::<Vec<u64>>();
        
        let space_rows = (0..grid.len())
            .map(|r| if is_row_all(grid, r, space_char) { spacing } else { 1 })
            .scan(0, |acc, x| {
                *acc += x;
                Some(*acc)
            }).collect::<Vec<u64>>();

        let mut galaxies = Vec::new();
        for (row_idx, row) in grid.iter().enumerate() {
            for (col_idx, &ch) in row.iter().enumerate() {
                if ch == galaxy_ch {
                    galaxies.push(Coord { row: space_rows[row_idx], col: space_cols[col_idx] });
                }
            }
        }
        galaxies
    }

    pub fn solution(fpath: &str) -> u64 {
        let grid = utilities::io::read_char_grid(fpath);
        let galaxies = get_galaxies(&grid, '#', '.', 2);

        // loop thru all combinations of galaxies and sum their distances
        let mut sum: u64 = 0;
        for (i, galaxy) in galaxies.iter().enumerate() {
            for other_galaxy in &galaxies[i+1..] {
                sum += galaxy.distance(other_galaxy);
            }
        }

        sum
    }
}

mod part2 {
    use crate::part1::{get_galaxies, Distance};

    pub fn solution(fpath: &str) -> u64 {
        let grid = utilities::io::read_char_grid(fpath);
        let galaxies = get_galaxies(&grid, '#', '.', 1000000);

        // loop thru all combinations of galaxies and sum their distances
        let mut sum: u64 = 0;
        for (i, galaxy) in galaxies.iter().enumerate() {
            for other_galaxy in &galaxies[i+1..] {
                sum += galaxy.distance(other_galaxy);
            }
        }

        sum
    }
}
