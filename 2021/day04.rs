use std::io;

struct Cell {
    value: u32,
    marked: bool,
}

type Row = Vec<Cell>;
type Grid = [Row; 5];

fn parse_row() -> Row {
    let mut input = String::new();
    io::stdin().read_line(&mut input);
    input.trim_end().split_whitespace()
        .map(|x| Cell {
            value: x.parse::<u32>().unwrap(),
            marked: false
        }).collect()
}

fn parse_grid() -> Option<Grid> {
    let mut input = String::new();
    match io::stdin().read_line(&mut input) {
        Err(_) => None,
        Ok(0) => None,
        Ok(_) => Some([
            parse_row(),
            parse_row(),
            parse_row(),
            parse_row(),
            parse_row(),
        ]),
    }
}

fn check_row(row: &Row) -> bool {
    row.iter().all(|c| c.marked)
}

fn check_col(grid: &Grid, i: usize) -> bool {
    grid.iter().all(|r| r[i].marked)
}

fn output(grid: &Grid, v: u32) -> () {
    let sum = grid.iter()
        .fold(0,
              |st, r|
              st + r.iter().fold(0,
                                 |sr, c|
                                 sr + if c.marked {0} else {c.value}));
    println!("{} * {} = {}", sum, v, sum * v);
}

fn main() -> io::Result<()> {
    let mut input = String::new();
    let mut first = true;
    io::stdin().read_line(&mut input);
    let draw : Vec<u32> = input
        .trim_end()
        .split(",")
        .map(|x| x.parse::<u32>().unwrap())
        .collect();
    let mut tables = Vec::<Grid>::new();
    loop {
        let grid = parse_grid();
        match grid {
            None => break,
            Some(g) => tables.push(g),
        }
    }
    let mut last: Option<Grid> = None;
    let mut xvalue = 0;
    for v in draw {
        let len = tables.len();
        let mut rit = Vec::new();
        for (it, t) in tables.iter_mut().enumerate() {
            'inner: for r in t.iter_mut() {
                for (ir, c) in r.iter_mut().enumerate() {
                    if c.value == v {
                        c.marked = true;
                        if check_row(&r) || check_col(&t, ir) {
                            if first {
                                output(&t, v);
                                first = false;
                            }
                            rit.push(it);
                            xvalue = v;
                        }
                        break 'inner;
                    }
                }
            }
        }
        for i in rit.iter().rev() {
            last = Some(tables.swap_remove(*i));
        }
        if tables.is_empty() {
            break;
        }
    }
    output(&last.unwrap(), xvalue);
    Ok(())
}
