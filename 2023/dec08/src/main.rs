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

    { /* Example 2 */
        print!("Example 1 ");
        let timer = std::time::Instant::now();
        let ans = part2::solution("example-input-3.txt");
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
    use std::collections::HashMap;

    #[derive(Clone)]
    pub struct Node {
        pub val: String,
        pub left: String,
        pub right: String
    }

    pub fn parse_path(lines: &[String]) -> HashMap<String, Node> {
        let mut graph: HashMap<String, Node> = HashMap::new();

        for line in lines {
            let node = Node {
                val: line[0..3].to_string(),
                left: line[7..10].to_string(),
                right: line[12..15].to_string()
            };

            graph.insert(node.val.clone(), node);
        }

        graph
    }

    pub fn solution(fpath: &str) -> u64 {
        let lines = utilities::io::read_file_lines(fpath);

        let directions: Vec<char> = lines[0].chars().collect();
        let graph = parse_path(&lines[2..]);

        let mut node = graph.get("AAA").expect("no AAA node");
        let mut idx: u64 = 0;
        while node.val != "ZZZ" {
            let dir = directions[(idx as usize) % directions.len()];
            idx += 1;

            node = match dir {
                'R' => graph.get(&node.right).expect("invalid node"),
                'L' => graph.get(&node.left).expect("invalid node"),
                _ => panic!("invalid direction")
            };
        }
        idx
    }
}


mod part2 {
    use std::collections::HashMap;
    use crate::part1::{parse_path, Node};

    fn steps_to_end(start: &Node, graph: &HashMap<String, Node>, directions: &Vec<char>) -> u64 {
        let mut idx = 0;
        let mut node = start;
        while !node.val.ends_with("Z") {
            let dir = directions[(idx as usize) % directions.len()];
            idx += 1;

            node = match dir {
                'R' => graph.get(&node.right).expect("invalid node"),
                'L' => graph.get(&node.left).expect("invalid node"),
                _ => panic!("invalid direction")
            };
        }
        idx
    }

    fn lcm(a: u64, b: u64) -> u64 {
        if a == 0 || b == 0 {
            return 0;
        }
        let gcd = gcd(a, b);
        a * b / gcd
    }

    fn gcd(a: u64, b: u64) -> u64 {
        if b == 0 {
            return a;
        }
        return gcd(b, a % b);
    }

    pub fn solution(fpath: &str) -> u64 {
        let lines = utilities::io::read_file_lines(fpath);

        let directions: Vec<char> = lines[0].chars().collect();
        let graph = parse_path(&lines[2..]);

        let nodes: Vec<Node> = graph.keys()
            .filter(|s| s.ends_with("A"))
            .map(|n| graph.get(n).expect("invalid node")).cloned().collect();

        nodes.iter()
            .map(|n| steps_to_end(n, &graph, &directions))
            .fold(1, |result, x| lcm(result, x))
    }
}