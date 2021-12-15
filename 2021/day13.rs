use std::io;
use std::io::Write;

/* special input: always fold in the middle */
fn do_fold<const N: usize>(paper: &mut [[bool; N]; N],
                           xmax: &mut usize, ymax: &mut usize) -> bool {
    let mut line = String::new();
    match io::stdin().read_line(&mut line) {
        Ok(1) | Ok(0) | Err(_) => false,
        Ok(_) => {
            let mut pair = line.trim_end().split('=');
            let instruction = pair.next().unwrap();
            let xy = pair.next().unwrap().parse::<usize>().unwrap();
            if instruction == "fold along x" {
                let mut xupward = xy;
                let mut xdownward = xy;
                'xcheck: loop {
                    for c in 0..*ymax {
                        if paper[xupward][c] || paper[xdownward][c] {
                            break 'xcheck;
                        }
                    }
                    xupward -= 1;
                    xdownward += 1;
                }
                if *xmax - xdownward > xupward + 1 {
                    panic!("unhandled");
                }
                for i in 0..(*xmax - xdownward) {
                    for c in 0..*ymax {
                        if paper[xdownward + i][c] {
                            paper[xupward - i][c] = true;
                        }
                    }
                }
                *xmax = xupward + 1;
            } else {
                let mut yleft = xy;
                let mut yright = xy;
                'ycheck: loop {
                    for r in 0..*xmax {
                        if paper[r][yleft] || paper[r][yright] {
                            break 'ycheck;
                        }
                    }
                    yleft  -= 1;
                    yright += 1;
                }
                if *ymax - yright > yleft + 1 {
                    panic!("unhandled: ymax: {}, yright: {}, yleft: {}",
                           *ymax, yright, yleft);
                }
                for r in 0..*xmax {
                    for i in 0..(*ymax - yright) {
                        if paper[r][yright + i] {
                            paper[r][yleft - i] = true;
                        }
                    }
                }
                *ymax = yleft + 1;
            }
            true
        }
    }
}

fn main() -> io::Result<()> {
    let mut paper = [[false; 1500]; 1500];
    let mut xmax = 0;
    let mut ymax = 0;
    loop {
        let mut line = String::new();
        match io::stdin().read_line(&mut line) {
            Ok(1) | Ok(0) | Err(_) => break,
            Ok(_) => {
                let mut pair = line.trim_end().split(',')
                    .map(|s| s.parse::<usize>().unwrap());
                let x = pair.next().unwrap();
                let y = pair.next().unwrap();
                if x > xmax { xmax = x; }
                if y > ymax { ymax = y; }
                paper[x][y] = true;
            }
        }
    }
    xmax = xmax + 1;
    ymax = ymax + 1;
    do_fold(&mut paper, &mut xmax, &mut ymax);
    let mut num = 0;
    for r in 0..xmax {
        for c in 0..ymax {
            if paper[r][c] { num += 1; }
        }
    }
    println!("{}", num);
    while do_fold(&mut paper, &mut xmax, &mut ymax) {
    }
    for c in 0..ymax {
        for r in 0..xmax {
            if paper[r][c] {
                io::stdout().write(&b"#"[..]);
            } else {
                io::stdout().write(&b" "[..]);
            }
        }
        println!("{}", "");
    }
    Ok(())
}
