import { useState } from 'react'

import './App.css'
const rows = 10
const cols = 10

function Cell({x,y,cellClass}) {
    return (<button className={cellClass}>{x} {y}</button>);
}
function Grid({rows, cols, start, end, obstacles, path}) {
    const grid = [];
    const obstacleSet = new Set(obstacles.map(o => `${o.x},${o.y}`));
    const pathSet = new Set(path.map(o => `${o.x},${o.y}`));
    for (let j = 0; j < cols; j++) {
        for (let i = 0; i < rows; i++) {
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
                <Cell key={`${i}-${j}`} x={i} y={j} cellClass={cellClass}/>
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
    const [obstacles, setObstacles] = useState(
        [
            {"x":2,"y":3},
            {"x":9,"y":8},
            {"x":12,"y":5},
            {"x":9,"y":14},
            {"x":8,"y":16},
            {"x":8,"y":10},
            {"x":11,"y":10},
            {"x":8,"y":12},
            {"x":10,"y":12},
            {"x":7,"y":12},
            {"x":8,"y":14},
            {"x":10,"y":15},
            {"x":7,"y":14},
            {"x":10,"y":17},
        ]
    );
    const [path, setPath] = useState([]);
    const [distance, setDistance] = useState(0);
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
  return (
      <div>
          <Grid rows={rows} cols={cols} start={start} end={end} obstacles={obstacles} path={path}/>
          <button onClick={()=>handleSubmit()}>Run</button>
      </div>
  )
}

export default App
