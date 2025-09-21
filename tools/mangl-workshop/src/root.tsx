// import React from 'react';
// import ReactDOM from 'react-dom/client';
// import './index.css';
// import App from './App';
// import reportWebVitals from './reportWebVitals';

// import { createRoot } from 'react-dom/client';
// import { BrowserRouter, Route, Routes } from 'react-router-dom';

// import { GlobalStyles } from '@mui/material';
// import { ThemeProvider } from '@mui/material/styles';


// import Home from './home'


// const root = ReactDOM.createRoot(
//   document.getElementById('root') as HTMLElement
// );
// root.render(
//   <BrowserRouter>
//     <Routes>
//       <Route path='/' element={<Home />} />
//     </Routes>
//   </BrowserRouter>
// );

// // If you want to start measuring performance in your app, pass a function
// // to log results (for example: reportWebVitals(console.log))
// // or send to an analytics endpoint. Learn more: https://bit.ly/CRA-vitals
// reportWebVitals();


import React from 'react'
import { createRoot } from 'react-dom/client'

import { Box, GlobalStyles, Typography } from '@mui/material'
import { CssBaseline, ThemeProvider } from '@mui/material'

import { muiTheme_, globalStyles } from './theme'
import HomePage from './home'




const container = document.getElementById('root')!
const root = createRoot(container)

const inputGlobalStyles = <GlobalStyles styles={globalStyles} />

root.render(
  <Box>
    <ThemeProvider theme={muiTheme_}>
      <HomePage />
    </ThemeProvider>
  </Box>

)
