import { useState } from 'react'

import './App.css'
const rows = 10
const cols = 10

function Cell({x,y,cellClass, onMouseDown, onMouseEnter}) {
    return (<button className={`cell ${cellClass}`}
            onMouseDown={onMouseDown}
            onMouseEnter={onMouseEnter}
            onDragStart={(e) => e.preventDefault()}
            >{x} {y}</button>);
}
function Grid({rows, cols, start, end, obstacles, path,onMouseDown, onMouseEnter}) {
    const grid = [];
    const obstacleSet = new Set(obstacles.map(o => `${o.x},${o.y}`));
    const pathSet = new Set(path.map(o => `${o.x},${o.y}`));
    for (let i = 0; i < rows; i++) {
        for (let j = 0; j < cols; j++) {
            let cellClass = "";
            if(start.x === i && start.y === j) cellClass = "start";
            else if(end.x === i && end.y === j) cellClass = "end";
            else if (obstacleSet.has(`${i},${j}`)) {
                cellClass = "obstacle";
            }
            else if (pathSet.has(`${i},${j}`)) {
                cellClass = "path";
            }
            grid.push(
                <Cell key={`${i}-${j}`} x={i} y={j} cellClass={cellClass}
                      onMouseDown={() => onMouseDown(i, j)}
                      onMouseEnter={() => onMouseEnter(i, j)}/>
            );
        }
    }
    const gridStyle = {
        display: 'grid',
        gridTemplateColumns: `repeat(${cols}, 1fr)`,
        gridTemplateRows: `repeat(${rows}, 1fr)`,
        width: '100vw',
        height: '90vh',
        gap: '1px',
        boxSizing: 'border-box'
    };
    return <div style={gridStyle}>{grid}</div>;
}



function App() {
    const rows = 20;
    const cols = 20;
    const [matrix, setMatrix] = useState([]);
    const [start, setStart] = useState({"x":7,"y":16});
    // const [start, setStart] = useState({"x":Math.floor(Math.random()*cols),"y":Math.floor(Math.random()*rows)});
    const [end, setEnd] = useState({"x":10,"y":10});
    const [obstacles, setObstacles] = useState([]);
    const [path, setPath] = useState([]);
    const [distance, setDistance] = useState(0);
    const [isMouseDown, setIsMouseDown] = useState(false);

    const handleSubmit = async (event) => {
        const req = {
            "width": cols,
            "height": rows,
            "start": start,
            "end": end,
            "obstacles": obstacles
        }
        const apiUrl = 'http://127.0.0.1:18080/api/path';
        // Define the request options
        const requestOptions = {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(req)
        };

        try {
            // Send the POST request
            const response = await fetch(apiUrl, requestOptions);

            // Check for HTTP errors
            if (!response.ok) {
                const errorData = await response.json();
                throw new Error(errorData.message || 'Failed to send POST request.');
            }

            // Parse the JSON response
            const data = await response.json();
            console.log(data);
            setPath(data.path);
            setDistance(data.distance);
        } catch (error) {
            // Handle any errors that occurred during the fetch operation
            // setError(error.toString());
            console.error('There was an error!', error);
        }
    };


    const toggleObstacle = (x, y) => {
        // Prevent painting over start or end
        if ((x === start.x && y === start.y) || (x === end.x && y === end.y)) return;

        setObstacles((prev) => {
            const exists = prev.some(o => o.x === x && o.y === y);
            if (exists) return prev.filter(o => !(o.x === x && o.y === y));
            return [...prev, { x, y }];
        });
    };

    const handleMouseDown = (x, y) => {
        setIsMouseDown(true);
        toggleObstacle(x, y);
    };

    const handleMouseEnter = (x, y) => {
        if (!isMouseDown) return; // Only paint if mouse is held down
        toggleObstacle(x, y);
    };

    const handleMouseUp = () => {
        setIsMouseDown(false);
    };

  return (
      <div style={{ display: 'flex', flexDirection: 'column', height: '100vh' }} onMouseUp={handleMouseUp} onMouseLeave={handleMouseUp}>
          <Grid rows={rows} cols={cols} start={start} end={end} obstacles={obstacles} path={path}
                onMouseDown={handleMouseDown}
                onMouseEnter={handleMouseEnter}/>
          <div style={{ height: '10vh', display: 'flex', alignItems: 'center', justifyContent: 'center' }}>
              <button onClick={handleSubmit} style={{ padding: '10px 20px' }}>Run Pathfinding</button>
          </div>
      </div>
  )
}

export default App
