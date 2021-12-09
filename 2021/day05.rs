use std::io;

struct Point {
    x: usize,
    y: usize,
}

struct Pair {
    l: Point,
    r: Point,
}

fn parse_pair(line: &str) -> Pair {
    let mut it = line.split_whitespace();
    let mut f = it.next().unwrap().split(',').map(|x| x.parse::<usize>());
    let fx = f.next().unwrap().unwrap();
    let fy = f.next().unwrap().unwrap();
    it.next();
    let mut t = it.next().unwrap().split(',').map(|x| x.parse::<usize>());
    let tx = t.next().unwrap().unwrap();
    let ty = t.next().unwrap().unwrap();
    Pair {
        l: Point { x : fx, y : fy, },
        r: Point { x : tx, y : ty, },
    }
}

fn main() -> io::Result<()> {
    let mut p1 = [[0;1000]; 1000];
    let mut p2 = [[0;1000]; 1000];
    loop {
        let mut input = String::new();
        match io::stdin().read_line(&mut input) {
            Err(_) => break,
            Ok(0) => break,
            Ok(_) => {
                let pair = parse_pair(&input);
                if pair.l.x == pair.r.x && pair.l.y == pair.r.y {
                    p1[pair.l.x][pair.l.y] += 1;
                    p2[pair.l.x][pair.l.y] += 1;
                } else if pair.l.x == pair.r.x {
                    for y in pair.l.y..(pair.r.y+1) {
                        p1[pair.l.x][y] += 1;
                        p2[pair.l.x][y] += 1;
                    }
                    for y in pair.r.y..(pair.l.y+1) {
                        p1[pair.l.x][y] += 1;
                        p2[pair.l.x][y] += 1;
                    }
                } else if pair.l.y == pair.r.y {
                    for x in pair.l.x..(pair.r.x+1) {
                        p1[x][pair.l.y] += 1;
                        p2[x][pair.l.y] += 1;
                    }
                    for x in pair.r.x..(pair.l.x+1) {
                        p1[x][pair.l.y] += 1;
                        p2[x][pair.l.y] += 1;
                    }
                } else {
                    if pair.l.x + pair.r.y == pair.l.y + pair.r.x {
                        if pair.r.x > pair.l.x {
                            for i in 0..(pair.r.x - pair.l.x + 1) {
                                p2[pair.l.x + i][pair.l.y + i] += 1;
                            }
                        } else {
                            for i in 0..(pair.l.x - pair.r.x + 1) {
                                p2[pair.r.x + i][pair.r.y + i] += 1;
                            }
                        }
                    } else if pair.l.x + pair.l.y == pair.r.x + pair.r.y {
                        if pair.r.x > pair.l.x {
                            for i in 0..(pair.r.x - pair.l.x + 1) {
                                p2[pair.l.x + i][pair.l.y - i] += 1;
                            }
                        } else {
                            for i in 0..(pair.l.x - pair.r.x + 1) {
                                p2[pair.r.x + i][pair.r.y - i] += 1;
                            }
                        }
                    }
                }
            },
        }
    }
    let sum = |p: &[[i32; 1000]; 1000]| {
        p.iter().fold(0, |st, r|
            st + r.iter().fold(0, |sr, c|
                sr + if c > &1 {1} else {0}))
    };
    println!("{}", sum(&p1));
    println!("{}", sum(&p2));
    Ok(())
}
