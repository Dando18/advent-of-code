fn main() {
    { /* Example 1.1 */
        print!("Example 1.1 ");
        let timer = std::time::Instant::now();
        let ans = part1::solution("example-input-1.txt");
        println!("({:?}):\t{}", timer.elapsed(), ans);
    }

    { /* Example 1.2 */
        print!("Example 1.2 ");
        let timer = std::time::Instant::now();
        let ans = part1::solution("example-input-2.txt");
        println!("({:?}):\t{}", timer.elapsed(), ans);
    }

    { /* Part 1 */
        print!("Part 1 ");
        let timer = std::time::Instant::now();
        let ans = part1::solution("input.txt");
        println!("({:?}):\t{}", timer.elapsed(), ans);
    }

    { /* Example 2.1 */
        print!("Example 2.1 ");
        let timer = std::time::Instant::now();
        let ans = part2::solution("example-input-3.txt");
        println!("({:?}):\t{}", timer.elapsed(), ans);
    }

    { /* Example 2.2 */
        print!("Example 2.2 ");
        let timer = std::time::Instant::now();
        let ans = part2::solution("example-input-4.txt");
        println!("({:?}):\t{}", timer.elapsed(), ans);
    }

    { /* Example 2.3 */
        print!("Example 2.3 ");
        let timer = std::time::Instant::now();
        let ans = part2::solution("example-input-5.txt");
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

    trait Neighbors: Sized {
        fn above(&self) -> Option<Self>;
        fn below(&self) -> Option<Self>;
        fn left(&self) -> Option<Self>;
        fn right(&self) -> Option<Self>;
    }

    #[derive(PartialEq, Eq, Copy, Clone, Debug)]
    pub struct Position {
        pub row: usize,
        pub col: usize
    }

    impl Neighbors for Position {
        fn above(&self) -> Option<Position> {
            if self.row == 0 {
                None
            } else {
                Some(Position { row: self.row - 1, col: self.col })
            }
        }

        fn below(&self) -> Option<Position> {
            Some(Position { row: self.row + 1, col: self.col })
        }

        fn left(&self) -> Option<Position> {
            if self.col == 0 {
                None
            } else {
                Some(Position { row: self.row, col: self.col - 1 })
            }
        }

        fn right(&self) -> Option<Position> {
            Some(Position { row: self.row, col: self.col + 1 })
        }
    }

    pub fn get_source_idx(lines: &Vec<Vec<char>>) -> Option<Position> {
        for (row_idx, row) in lines.iter().enumerate() {
            let src_loc = row.iter().position(|&c| c == 'S');
            if src_loc.is_some() {
                return Some(Position { row: row_idx, col: src_loc.unwrap() });
            }
        }
        None
    }

    pub fn find_source_neighbors(grid: &Vec<Vec<char>>, pos: &Position) -> [Position; 2] {
        let mut vals: Vec<Position> = Vec::new();

        if let Some(left) = pos.left() {
            let left_val: char = grid[left.row][left.col];
            if left_val == 'F' || left_val == 'L' || left_val == '-' {
                vals.push(left);
            }
        }
        
        if let Some(right) = pos.right() {
            let right_val: char = grid[right.row][right.col];
            if right_val == 'J' || right_val == '7' || right_val == '-' {
                vals.push(right);
            }
        }

        if let Some(above) = pos.above() {
            let above_val: char = grid[above.row][above.col];
            if above_val == '|' || above_val == '7' || above_val == 'F' {
                vals.push(above);
            }
        }

        if let Some(below) = pos.below() {
            let below_val: char = grid[below.row][below.col];
            if below_val == 'L' || below_val == 'J' || below_val == '|' {
                vals.push(below);
            }
        }

        assert_eq!(vals.len(), 2);
        vals.try_into().expect("too many neighbors of source")
    }

    pub fn walk(grid: &Vec<Vec<char>>, cur: &Position, prev: Option<&Position>) -> Position {
        let cur_pipe = grid[cur.row][cur.col];

        let avail: [Option<Position>; 2] = match cur_pipe {
            '|' => [cur.above(), cur.below()],
            '-' => [cur.left(), cur.right()],
            'L' => [cur.above(), cur.right()],
            'J' => [cur.above(), cur.left()],
            '7' => [cur.left(), cur.below()],
            'F' => [cur.below(), cur.right()],
            'S' => find_source_neighbors(grid, cur).map(Some),
            _ => panic!("on an invalid pipe")
        };

        if prev.is_none() {
            avail[0].expect("invalid node")
        } else if avail[0].is_none() || prev.unwrap().to_owned() == avail[0].unwrap() {
            avail[1].expect("invalid node")
        } else {
            avail[0].expect("invalid node")
        }
    }

    pub fn solution(fpath: &str) -> u32 {
        let grid = utilities::io::read_char_grid(fpath);
        let src = get_source_idx(&grid).expect("no source found");

        let mut pos = walk(&grid, &src, None);
        let mut prev = src.clone();

        let mut n_steps: u32 = 0;
        while pos != src {
            let next = walk(&grid, &pos, Some(&prev));
            prev = pos.clone();
            pos = next;
            n_steps += 1;
        }

        (n_steps + 1) / 2   // ceil(n/2)
    }
}

mod part2 {
    use crate::part1::{walk, get_source_idx};
    use geo::{Polygon, LineString, Contains, point};

    fn get_loop(grid: &Vec<Vec<char>>) -> Polygon<i64> {
        let mut corners: Vec<(i64, i64)> = Vec::new();

        let src = get_source_idx(grid).expect("no source found");
        corners.push((src.row as i64, src.col as i64));

        let mut pos = walk(grid, &src, None);
        let mut prev = src.clone();
        corners.push((pos.row as i64, pos.col as i64));

        while pos != src {
            let next = walk(&grid, &pos, Some(&prev));
            prev = pos.clone();
            pos = next;
            corners.push((pos.row as i64, pos.col as i64));
        }

        Polygon::new(
            LineString::from(corners),
            vec![]
        )
    }

    pub fn solution(fpath: &str) -> u32 {
        let grid = utilities::io::read_char_grid(fpath);
        let polygon = get_loop(&grid);

        grid.iter().enumerate()
            .flat_map(|(row_idx, row)| {
                row.iter().enumerate().map(move |(col_idx, val)| (row_idx.clone(), col_idx.clone(), val.clone()))
            })
            .filter(|coord| {
                polygon.contains(&point!(x: coord.0 as i64, y: coord.1 as i64))
            }).count().try_into().expect("convert error")
    }
}
