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

    pub struct Number {
        pub value: u32,
        pub cells: Vec<(usize,usize)>
    }

    fn is_symbol(c: char) -> bool {
        !c.is_digit(10) && c != '.'
    }

    fn get_cell(grid: &Vec<Vec<char>>, row_idx: i32, col_idx: i32) -> Option<char> {
        if row_idx < 0 || col_idx < 0 {
            None
        } else {
            grid.get(row_idx as usize).and_then(|r| r.get(col_idx as usize)).copied()
        }
    }

    fn has_adjacent_symbol(grid: &Vec<Vec<char>>, row_idx: usize, col_idx: usize) -> bool {
        let offsets = [[-1,-1], [-1,0], [-1,1],
                       [0,-1],          [0,1],
                       [1,-1],  [1,0],  [1,1]];
        
        let mut has_adj = false;
        for offset in offsets {
            let cell = get_cell(grid, (row_idx as i32) + offset[0], (col_idx as i32) + offset[1]);
            if cell.is_some() && is_symbol(cell.unwrap()) {
                has_adj = true;
                break;
            }
        }
        has_adj
    }

    pub fn get_numbers(grid: &Vec<Vec<char>>) -> Vec<Number> {
        let mut cells: Vec<Number> = Vec::new();
        for (row_idx, row) in grid.iter().enumerate() {
            let mut col_idx = 0;
            while col_idx < row.len() {
                let cur_char: char = row[col_idx];

                if cur_char.is_digit(10) {
                    /* scan to end of number */
                    let mut substr: String = String::new();
                    let mut positions: Vec<(usize,usize)> = Vec::new();

                    while col_idx < row.len() && row[col_idx].is_digit(10) {
                        substr.push(row[col_idx]);
                        positions.push((row_idx, col_idx));
                        col_idx += 1;
                    }

                    cells.push(Number {
                        value: substr.parse().expect("failed to parse number"),
                        cells: positions
                    });
                }

                col_idx += 1;
            }
        }
        cells
    }

    pub fn solution(fpath: &str) -> u32 {
        let grid = utilities::io::read_char_grid(fpath);
        let cells = get_numbers(&grid);

        /* now sum numbers with adjacent symbols */
        let mut sum: u32 = 0;
        for number in cells {
            let mut does_number_have_adjacent_symbol: bool = false;
            for cell in number.cells {
                if has_adjacent_symbol(&grid, cell.0, cell.1) {
                    does_number_have_adjacent_symbol = true;
                    break;
                }
            }

            if does_number_have_adjacent_symbol {
                sum += number.value;
            }
        }

        sum
    }
}

mod part2 {
    use crate::part1;

    fn is_adjacent(number: &part1::Number, row_idx: usize, col_idx: usize) -> bool {
        for pos in number.cells.iter() {
            if ((pos.0 as i64) - (row_idx as i64)).abs() <= 1 && ((pos.1 as i64) - (col_idx as i64)).abs() <= 1 {
                return true;
            }
        }
        return false;
    }

    pub fn solution(fpath: &str) -> u64 {
        let grid = utilities::io::read_char_grid(fpath);
        let numbers = part1::get_numbers(&grid);

        let mut sum: u64 = 0;
        for (row_idx, row) in grid.iter().enumerate() {
            for (col_idx, value) in row.iter().enumerate() {
                if value.clone() != '*' {
                    continue;
                }

                let adj_numbers: Vec<&part1::Number> = numbers.iter()
                    .filter(|n| is_adjacent(n, row_idx, col_idx))
                    .collect();
            
                if adj_numbers.len() == 2 {
                    sum += adj_numbers.iter().map(|n| n.value as u64).product::<u64>();
                }

            }
        }

        sum
    }
}
